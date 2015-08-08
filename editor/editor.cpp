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

    explicit Field(const std::string& text, int idValue, FieldType type, int startLine, int startCol, int size = Unlimited);
    ~Field();
    bool Insert(char c, int line, int col);
    void Insert(const std::string& text, int line, int col);
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
    int m_size;      //Tamanho máximo de caracteres, caso não tenha limites deve ser -1
    FieldType m_type;//Tipo do campo
    std::vector<char> m_characters; //Caracteres

    ///Variáveis relacionadas ao tipo de objeto
    ///////////////////////////////////////////
    int m_idValue;//Melhorar esse nome
};

Field::Field(const std::string& text, int idValue, FieldType type, int startLine, int startCol, int size) :
    m_startLine(startLine),
    m_endLine(startLine),
    m_startCol(startCol),
    m_size(size),
    m_type(type),
    m_idValue(idValue)
{
    this->Insert(text, startLine, startCol);
}

Field::~Field()
{
    m_characters.clear();
}

bool Field::Insert(char c, int line, int col)
{
    if( c != '\n' && (m_size != Unlimited && (m_characters.size() + 1) > (size_t)m_size) )
    {
        return false;
    }

    /// string TESTE\nDE
    /// coluna 12345 678
    /// linha  00000  11
    ///
    std::string::const_iterator it;
    int startCol = 0;

    if( m_startCol == 0 )
    {
        it = m_characters.end();
    }
    else
    {
        startCol = this->GetStartColumn(line);
        if( startCol == 0 )
        {
            it = m_characters.end();
        }
        else
        {
            col = col - m_startCol;
            it = m_characters.begin() + col;
        }
    }

    //std::cout << "[Field][Insert][m_startcol]" << "["<< m_startCol <<"]" << "["<< col <<"]" << "["<< startCol <<"]"<< std::endl;


    /*if( startCol == 0 )
    {
        if(col > m_size)
        {
            it = m_characters.end();
        }
    }
    else
    {
        col = col + startCol;
        it = m_characters.begin()+col;
    }*/

    m_characters.insert(it, 1, c);

    if(c == '\n' && this->m_type == Text)
    {
        m_endLine++;
    }

    return true;
}

void Field::Insert(const std::string& text, int line, int col)
{//Adicionar checagens?
    col = col + this->GetStartColumn(line);
    m_characters.resize(m_characters.size() + text.size() + 1);
    m_characters.insert(m_characters.end(), text.begin(), text.end());

    if(m_type == Title)
    {
        m_endCol = text.size() - 1;
    }
}

bool Field::Remove(int line, int col)
{
    if( (size_t)col > m_characters.size() )
    {
        return false;
    }

    auto it = m_characters.begin();
    col = col + GetStartColumn(line);
    m_characters.erase(it+col, it+col+1);
    return true;
}

int Field::Line()
{
    return m_endLine;
}

///////////////////////////////////////////////////////
/// 0       9                26       35
/// Título: Meu eu Boguremeu Título2: Nova coisa feita
/// ^^^^^^^ ^^^^^^^^^^^^^^^^ ^^^^^^^^ ^^^^^^^^^^^^^^^^
/// 1111111 2222222222222222 33333333 4444444444444444
/// Coluna 16
bool Field::Intersect(int line, int col)
{
    //std::cout << "[Intersect]["<< ToString() <<"]" << "["<< m_startCol <<"]" << "["<< m_endCol <<"]" << std::endl;

    if(line < m_startLine || line > m_endLine)
    {
        return false;
    }

    if( m_size != Unlimited )
    {
        if( col < m_startCol || col > m_endCol )
        {
            return false;
        }
    }

    return true;
}

int Field::GetStartColumn(int line)
{
    if(line == 0 || m_characters.size() == 0)
    {//Não tem necessidade de virtualizar as linhas
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

        if( lines == line )
        {
            break;
        }

        col++;
    }

    return col + 1;//Uma coluna após a quebra de linha
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
      m_currentColumn(0)
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
//    if( m_luaState )
//    {
//        lua_close(m_luaState);
//    }

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
    //std::cout << "line: " << m_currentLine << " Column: " << m_currentColumn << std::endl;

    auto field = GetLastFieldOfLine(m_currentLine);

    if( field == nullptr )
    {
        return;
    }

    field->Insert('\n', m_currentLine, m_currentColumn);
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
    //std::cout << "[GetField]" << "["<< m_fields.size() <<"]" << std::endl;
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
    unsigned int curColumn = m_currentColumn;//textCursor().columnNumber();
    unsigned int curLine = m_currentLine;//textCursor().blockNumber();

    if( Empty() == true )
    {
        return QObject::eventFilter(obj, event);
    }

    std::cout << "[eventFilter]" << "["<< curLine <<"]" << "["<< curColumn <<"]" << std::endl;
    Field* field = GetField(curLine, curColumn);
    if( field == nullptr )
    {
        UpdateCursor(curLine, curColumn);
        //std::cout << "Merda" << std::endl;
        return QObject::eventFilter(obj, event);
    }

    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        switch( keyEvent->key() )
        {
            case Qt::Key_CapsLock:
            case Qt::Key_Shift:
            {
                return QObject::eventFilter(obj, event);
            }

            case Qt::Key_Left:
            {
                (--m_currentColumn) < 0 ? m_currentColumn = 0 : m_currentColumn;
                UpdateCursor(m_currentLine, m_currentColumn);
                return QObject::eventFilter(obj, event);
            }
            case Qt::Key_Down:
            {
                (--m_currentLine) < 0 ? m_currentColumn = 0 : m_currentColumn;
                UpdateCursor(m_currentLine, m_currentColumn);
                return QObject::eventFilter(obj, event);
            }
            case Qt::Key_Right:
            {
                //(--m_currentColumn) < 0 ? m_currentColumn = 0 : m_currentColumn;
                UpdateCursor(m_currentLine, ++m_currentColumn);
                return QObject::eventFilter(obj, event);
            }
            case Qt::Key_Up:
            {
                UpdateCursor(++m_currentLine, m_currentColumn);
                return QObject::eventFilter(obj, event);
            }

            case Qt::Key_Enter:
            case Qt::Key_Return:
            {
                field->Insert('\n', curLine, curColumn);
                m_currentLine++;
                m_currentColumn = 0;
                emit Repaint();
                UpdateCursor(m_currentLine, m_currentColumn);
                return true;
            }

            case Qt::Key_Backspace:
            {
                if(textCursor().selectionStart() == textCursor().selectionEnd())
                {
                    field->Remove(curLine, curColumn - 1);
                    m_currentColumn--;
                }
                emit Repaint();
                return true;
            }

            case Qt::Key_Delete:
            {
                if(textCursor().selectionStart() == textCursor().selectionEnd())
                {
                    field->Remove(curLine, curColumn);
                }
                emit Repaint();
                return true;
            }

            default:
            {
                field->Insert(keyEvent->text().at(0).toLatin1(), curLine, curColumn);
                m_currentColumn++;
                emit Repaint();
                return true;
            }
        }
    }

    //Continua a execução normalmente
    return QObject::eventFilter(obj, event);
}

////////
/// Funções de auxílio
///
void Editor::CreateField(int objectValue, const std::string &text, int size)
{
    std::cout << "[CreateField]" << "["<< m_currentLine <<"]" << std::endl;
    Field* field = new Field(text, objectValue, Field::Title, m_currentLine, m_currentColumn, size);

    auto vec = Split(text, '\n');

    //Se o texto possui quebras de linha
    // atualizamos a linha corrente para refletir a última linha
    // e colocamos a coluna corrente para a posição zero
    if( vec.size() > 1 )
    {
        std::cout << "vec.size()" << vec.size() << std::endl;
        m_currentLine += vec.size();
        m_currentColumn = 0;
    }

    m_currentColumn += text.size();//Atualiza a coluna corrente

    this->m_fields.push_back(field);
}

void Editor::Paint()
{
    std::string text;
    clear();
    for( auto field : m_fields )
    {
        text.append(field->ToString());
    }


    //textCursor().insertText(QString::fromStdString(text));
    insertPlainText(QString::fromStdString(text));
    //UpdateCursor();
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

    CreateField(objectValue, text, text.size());

    return -1;
}

int Editor::Indent(lua_State *env)
{
    return -1;
}

}
