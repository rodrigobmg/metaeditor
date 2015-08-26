//System
#include <string>
#include <iostream>
#include <sstream>

//Qt
#include <QApplication>
#include <QTextBlock>

//Own
#include "editor.h"

namespace GUI
{

const int Unlimited = -1;

inline std::vector<std::string> &Split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}


inline std::vector<std::string> Split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    Split(s, delim, elems);
    return elems;
}

class Field
{
public:

    enum FieldType
    {
        Title,
        Name,
        Text,
        Relation
    };

    explicit Field(const std::string& text, int idValue, FieldType type = Text, int startLine = 0, int startCol = 0);
    ~Field();
    bool Insert(char c, int line, int col, bool updateEndCol = true);
    void Insert(const std::string& text);
    bool Remove(int line, int col);
    bool Intersect(int line, int col);
    int Line();
    std::string ToString();
    void Print();

private:
    int GetStartColumn(int line);

private:
    //Variáveis relacionadas ao texto
    int m_startLine; //Linha inicial
    int m_endLine;   //Linha final, caso não tenha enter na string deve ser igual a linha inicial
    int m_startCol;  //Coluna inicial na linha inicial
    int m_endCol;    //Coluna final do campo, normalmente utilizado para campos de título
    FieldType m_type;//Tipo do campo
    std::vector<char> m_characters; //Caracteres

    ///Variáveis relacionadas ao tipo de objeto
    ///////////////////////////////////////////
    int m_idValue;//Melhorar esse nome
};

Field::Field(const std::string& text, int idValue, FieldType type, int startLine, int startCol) :
    m_startLine(startLine),
    m_endLine(startLine),
    m_startCol(startCol),
    m_type(type),
    m_idValue(idValue)
{
    this->Insert(text);
}

Field::~Field()
{
    m_characters.clear();
}

bool Field::Insert(char c, int line, int col, bool updateEndCol)
{
    if( m_type == Text )
    {
        int startCol = this->GetStartColumn(line);
        col = startCol + col;

        if( c == '\n' )
        {
            m_endLine++;
        }
    }
    else
    {
        col = (m_startCol == 0 ? col : (col - m_startCol));
    }

    m_characters.insert(m_characters.begin() + col, c);

    if( updateEndCol )
    {
        m_endCol += 1;
    }

    return true;
}

void Field::Insert(const std::string& text)
{
    m_characters.insert(m_characters.begin(), text.begin(), text.end());

    if(m_type == Text)
    {
        auto vec = Split(text, '\n');
        m_endLine = m_startLine + (vec.size() - 1);
    }
    else
    {
        m_endCol = m_startCol + text.size();
    }
}

bool Field::Remove(int line, int col)
{
    if(m_type == Text)
    {
        col = col + GetStartColumn(line);
    }
    else
    {
        col -= m_startCol;
    }

    auto it = m_characters.begin();
    m_characters.erase(it+col);
    return true;
}

int Field::Line()
{
    return m_endLine;
}

bool Field::Intersect(int line, int col)
{
    if( line >= m_startLine && line <= m_endLine )
    {
        if( m_type == Text )
        {
            return true;
        }

        if(col >= m_startCol && col <= m_endCol)
        {
            return true;
        }
    }

    return false;
}

int Field::GetStartColumn(int line)
{
    if(line == 0 || m_characters.size() == 0)
    {
        return 0;
    }

    int lines = 0;
    int col = 0;
    for(auto s : m_characters)
    {
        if( s == '\n' )
        {
            lines++;
        }

        if( lines == (line - m_startLine) )
        {
            return col;
        }

        col++;
    }

    return col;
}

std::string Field::ToString()
{
    return std::string(m_characters.begin(), m_characters.end());
}

void Field::Print()
{
    std::cout << "debug print" << std::endl;

    for(auto s : m_characters)
    {
        switch(s)
        {
        case '\n': std::cout << "$"; break;
        case '\t': std::cout << "#"; break;
        default: std::cout << s;
        }
    }

    std::cout << std::endl;
}

/////////////////
///Editor de texto
Editor::Editor()
    : m_luaState(nullptr),
      m_currentLine(0),
      m_currentColumn(0),
      m_needsUpdate(false)
{
    CreateField(0, "teste de campo");
}

Editor::Editor(const std::string luaScript, int objectId)
    : m_currentObject(objectId),
      m_luaState(nullptr),
      m_currentLine(0),
      m_currentColumn(0)

{
    //Inicializa o ambiente Lua para este editor
    m_luaState = luaL_newstate();
    luaL_openlibs(m_luaState);

    Luna<Editor>::Register(m_luaState, this);

    //ToDo: Chamar o script lua
    luaL_dofile(m_luaState, luaScript.c_str());
}

Editor::~Editor()
{
    for(auto field : m_fields)
    {
        delete field;
    }

    m_fields.clear();
}

void Editor::UpdateCursor(int currentLine, int currentColumn)
{
    QTextBlock block = this->document()->findBlockByLineNumber(currentLine);
    QTextCursor cursor(block);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, currentColumn);
    setTextCursor(cursor);
    centerCursor();
    setFocus();
}

void Editor::JumpLine(int n)
{
    auto field = GetLastFieldOfLine(m_currentLine);

    if( field == nullptr )
    {
        return;
    }

    field->Insert(' ', m_currentLine, m_currentColumn);
    field->Insert('\n', m_currentLine, m_currentColumn + 1, false);//Mantém um espaço para inserção de novos dados
    field->Print();

    QTextBlock block = this->document()->findBlockByLineNumber(n);

    if(block.isValid() == false)
    {
        unsigned int lines = this->document()->lineCount();

        for( unsigned int i = lines; i <= n; i++)
        {
            textCursor().insertBlock();
        }

        block = this->document()->findBlockByLineNumber(n);
    }

    QTextCursor cursor(block);
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 0);
    setTextCursor(cursor);
    centerCursor();
    setFocus();

    m_currentLine++;
    m_currentColumn = 0;
}

Field* Editor::GetField(unsigned int line, unsigned int column)
{
    for( auto field : m_fields )
    {
        if( field->Intersect(line, column) == true )
        {
            std::cout << "[FIELD]" << "["<< field->ToString() <<"]" << std::endl;
            return field;
        }
    }

    return nullptr;
}

Field* Editor::GetLastFieldOfLine(unsigned int line)
{
    Field* ret = nullptr;
    std::vector<Field*>::const_iterator it;

    int i = 0;
    for(; i < m_fields.size(); i++ )
    {
        if( m_fields.at(i)->Line() == line )
        {
            break;
        }
    }

    for(; i < m_fields.size(); i++)
    {
        if( m_fields.at(i)->Line() != line )
        {
            break;
        }

        ret = m_fields.at(i);
    }

    //std::cout << "[GetLastField]["<< ret->ToString() <<"]" <<std::endl;
    return ret;
}

bool Editor::Empty() const
{
    return m_fields.empty();
}

////////
/// Funções herdadas
///
bool Editor::eventFilter(QObject* obj, QEvent * event)
{
    m_currentColumn = textCursor().columnNumber();
    m_currentLine = textCursor().blockNumber();

    if( Empty() == true )
    {
        return QObject::eventFilter(obj, event);
    }

    std::cout << "[eventFilter]" << "["<< m_currentLine <<"]" << "["<< m_currentColumn <<"]" << std::endl;
    Field* field = GetField(m_currentLine, m_currentColumn);
    if( field == nullptr )
    {
        return QObject::eventFilter(obj, event);
    }

    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        switch( keyEvent->key() )
        {
            case Qt::Key_CapsLock:
            case Qt::Key_Shift:
            case Qt::Key_Left:
            case Qt::Key_Down:
            case Qt::Key_Right:
            case Qt::Key_Up:
            {
                return QObject::eventFilter(obj, event);
            }

            case Qt::Key_Enter:
            case Qt::Key_Return:
            {
                field->Insert('\n', m_currentLine, m_currentColumn);
                emit Repaint();
                m_needsUpdate = true;
                return true;
            }

            case Qt::Key_Backspace:
            {
                if(textCursor().selectionStart() == textCursor().selectionEnd())
                {
                    field->Remove(m_currentLine, m_currentColumn);
                    m_needsUpdate = true;
                }
                emit Repaint();
                return true;
            }

            case Qt::Key_Delete:
            {
                if(textCursor().selectionStart() == textCursor().selectionEnd())
                {
                    field->Remove(m_currentLine, m_currentColumn);
                    m_needsUpdate = true;
                }
                emit Repaint();
                return true;
            }

            default:
            {
                field->Insert(keyEvent->text().at(0).toLatin1(), m_currentLine, m_currentColumn + 1);
                m_needsUpdate = true;
                emit Repaint();
                return true;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

////////
/// Funções de auxílio
///
void Editor::CreateField(int objectValue, const std::string &text)
{
    Field::FieldType type = Field::Title;
    if( objectValue == 4 )
    {
        type = Field::Text;
    }

    auto field = new Field(text, objectValue, type, m_currentLine, m_currentColumn);

    if( type == Field::Text)
    {
        auto vec = Split(text, '\n');
        m_currentLine += vec.size();
        m_currentColumn = 0;
    }
    else
    {
        m_currentColumn += text.size();//Atualiza a coluna corrente
        field->Insert(' ', m_currentLine, m_currentColumn, false);
    }

    this->m_fields.push_back(field);
    m_needsUpdate = true;
}

void Editor::Paint()
{
    if( m_needsUpdate )
    {
        std::string text;
        clear();
        std::cout << "text size " << this->document()->characterCount() << std::endl;
        for( auto field : m_fields )
        {
            auto str = field->ToString();
            std::cout << "string: " << str << " size: " << str.size() << std::endl;
            text.append(field->ToString());
        }

        std::cout << "size: " << text.size() << std::endl;
        insertPlainText(QString::fromStdString(text));
        m_needsUpdate = false;
    }
}

////////
///Definições Lua
///

//Nome utilizado dentro do script Lua
const char Editor::className[] = "Editor";
const Luna<Editor>::RegType Editor::Register[] =
{
    { "NextLine",       &Editor::NextLine },
    { "CreateField",    &Editor::CreateField },
    { "Indent",         &Editor::Indent },
    { 0 }
};

int Editor::NextLine(lua_State* /*env*/)
{
    static int chamadas = 0;
    std::cout << "[JumpLine]" << "["<< ++chamadas <<"]" << std::endl;
    JumpLine();
    return -1;
}

int Editor::CreateField(lua_State *env)
{
    int objectValue = luaL_checkinteger(env, 2);
    size_t size;
    const char* str = luaL_checklstring(env, 3, &size);
    std::string text(str, size);

    std::cout << "[Lua][CreateField] " << "value: " << objectValue << " text: " << text << std::endl;

    CreateField(objectValue, text);

    return -1;
}

int Editor::Indent(lua_State *env)
{
    return -1;
}

}
