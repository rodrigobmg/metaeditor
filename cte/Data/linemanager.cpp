#include "linemanager.h"

#include <exception>

#ifdef __LOG_TEST__
#include "../Utils/logfile.h"
static LogFile logFile("DataManager", "LogTest", true);
#endif

Field::Field() :
    m_position(0), m_type(Text), m_start(0), m_end(0)
{
    //empty
}

Field::Field(int pos, FieldType type, unsigned int start, unsigned int end) :
    m_position(pos), m_type(type), m_start(start), m_end(end)
{
    //empty
}

bool Field::atRange(unsigned int col)
{
    return col >= m_start && col <= m_end;
}

template<typename _Iterator1, typename _Iterator2>
inline size_t IncUtf8StringIterator(_Iterator1& it, const _Iterator2& last) {
    if(it == last) return 0;
    unsigned char c;
    size_t res = 1;
    for(++it; last != it; ++it, ++res) {
        c = *it;
        if(!(c&0x80) || ((c&0xC0) == 0xC0)) break;
    }

    return res;
}

inline size_t Utf8StringSize(const std::string& str)  {
    size_t res = 0;
    std::string::const_iterator it = str.begin();
    for(; it != str.end(); IncUtf8StringIterator(it, str.end()))
        res++;

    return res;
}

void Field::appendInfo(std::string c, unsigned int pos)
{
    m_info.insert( (pos-m_start), c);
    //std::cout << "appendInfo --> String[" << c.c_str() << "] Size[" << c.size() << "]" << std::endl;
    m_end += Utf8StringSize(c);
}

void Field::appendChar(char c, unsigned int pos)
{
    m_info.insert( (pos-m_start) , 1, c);
    m_end++;
}

void Field::removeAt(unsigned int pos)
{
    m_info.erase( (pos-m_start), 1);
    m_end--;
}

void Field::shiftRight()
{
    m_start++;
    m_end++;
}

void Field::shiftLeft()
{
    m_start--;
    m_end--;
}

Field* Line::searchAtCol(unsigned int col)
{
    for(auto d : m_fields)
    {
        if(d->atRange(col))
        {
            return d;
        }
    }

    return nullptr;
}

std::string Line::dump()
{
    std::string ret = "";
    for( unsigned int i = 0; i < m_fields.size(); i++ )
    {
        ret.append(m_fields[i]->m_info);
        ret.append(" ");
    }

    return ret;
}

LineManager::LineManager()
{
#ifdef __LOG_TEST__
    logFile.write("[DataMananager] ctor");
#endif
}

LineManager::~LineManager()
{
    if( m_lines.empty() == false )
    {
        for( auto line : m_lines )
        {
            delete line.second;
        }

        m_lines.clear();
    }
}

unsigned int LineManager::lineCount()
{
    return (unsigned int)m_lines.size();
}

std::string LineManager::dumpLine(unsigned int line)
{
#ifdef __LOG_TEST__
    logFile.write("[DataMananager] dumping line [%d]", line);
#endif
    return m_lines.at(line)->dump();
}

std::string LineManager::dumpLines()
{
    std::string ret = std::string("");
    for( auto line : m_lines )
    {
        ret.append(line.second->dump() + "\n");
    }
    return ret;
}

#ifdef __LOG_TEST__
void LineManager::dump()
{
    logFile.write("[DataManager] Dumping all lines");
    for( auto line : m_lines )
    {
        logFile.write("\t>>>Line[%d] Text[ %s]", (line.second->m_number + 1), line.second->dump().c_str() );
    }
}
#endif

void LineManager::removeCharacter(unsigned int col, unsigned int line)
{
    try
    {
#ifdef __LOG_TEST__
        logFile.write("[DataMananager] Removing character Line[%d] Column[%d]", line, col);
#endif

        Line* ptr = m_lines.at(line);

        Field* type = ptr->searchAtCol(col);

        if( type != nullptr )
        {
            type->removeAt(col - 1);

            for( unsigned int i = type->m_position + 1; i < ptr->m_fields.size(); i++ )
            {
                Field* o = ptr->m_fields.at(i);
                o->shiftLeft();
            }
        }
    }
    catch(std::out_of_range& ex)
    {
#ifdef __LOG_TEST__
        logFile.write("[DataMananager] Remove Character Error [%s]", ex.what());
#endif
    }
    catch(std::exception& ex)
    {
        throw ex;
    }
}

void LineManager::addTitle(const char* str, unsigned int col, unsigned int line)
{
    Line* ptr = nullptr;

    try
    {
#ifdef __LOG_TEST__
        logFile.write("[DataMananager] Inserting title [%s] at Line[%d] in Column[%d]", str, line, col);
#endif
        ptr = m_lines.at(line);
    }
    catch(std::out_of_range& /*ex*/)
    {
        ptr = new Line;
        ptr->m_number = line;
        m_lines.insert(std::make_pair(line, ptr));
#ifdef __LOG_TEST__
        logFile.write("[DataMananager] Inserting a new line [%d]", line);
#endif
    }

    Field* type = ptr->searchAtCol(col);

    if( type == nullptr )
    {
        type = new Field((unsigned int)ptr->m_fields.size(), Title, col, col);
        type->appendInfo(str, col);
#ifdef __LOG_TEST__
        logFile.write("[DataMananager] Appending new string [%s] with size [%d] at [%d] in line [%d]", str, type->m_end, col, line);
#endif
        ptr->m_fields.push_back(type);
    }
}

void LineManager::addName(const char* str, unsigned int col, unsigned int line)
{
    Line* ptr = nullptr;
    try
    {
#ifdef __LOG_TEST__
        logFile.write("[DataMananager] Inserting name [%s] at Line[%d] in Column[%d]", str, line, col);
#endif
        ptr = m_lines.at(line);
    }
    catch(std::out_of_range& /*ex*/)
    {
        ptr = new Line;
        ptr->m_number = line;
        m_lines.insert(std::make_pair(line, ptr));
#ifdef __LOG_TEST__
        logFile.write("[DataMananager] Inserting a new line [%d]", line);
#endif
    }

    Field* type = ptr->searchAtCol(col);

    if( type == nullptr )
    {
        type = new Field((unsigned int)ptr->m_fields.size(), Name, col, col);
        type->appendInfo(str, col);
#ifdef __LOG_TEST__
        logFile.write("[DataMananager] Appending new string [%s] with size [%d] at [%d] in line [%d]", str, type->m_end, col, line);
#endif
        ptr->m_fields.push_back(type);
    }
}

bool LineManager::isEditable(unsigned int col, unsigned int line)
{
    if(m_lines.empty())
    {
        return true;
    }

    try
    {
        std::cout << "m_lines.size() " << m_lines.size() << std::endl;
        Line* ptr = m_lines.at(line);
        Field* type = ptr->searchAtCol(col);

        if(type != nullptr)
        {
            if( type->m_type == Title )
                return false;
        }

        return true;
    }
    catch(std::out_of_range& ex)
    {
#ifdef __LOG_TEST__
        logFile.write("[DataMananager][isEditable] Expected error at eventFilter [%s]", ex.what());
#endif
        return true;
    }
}

std::string LineManager::type(unsigned int col, unsigned int line) throw(std::exception)
{
    if(m_lines.empty())
    {
        return "Text";
    }

    try
    {
        Line* ptr = m_lines.at(line);
        Field* type = ptr->searchAtCol(col);

        if(type != nullptr)
        {
            switch(type->m_type)
            {
                case Title:
                    return "Title";
                case Text:
                    return "Text";
                case Name:
                    return "Name";
                case Relation:
                    return "Relation";
                case None:
                case MaxFieldType:
                    return "Error";
            }
        }

        return "Text";
    }
    catch(std::out_of_range& ex)
    {
        //logFile.write("[DataMananager][type] Expected error at eventFilter [%s]", ex.what());
        return "Text";
    }
    catch(std::exception& ex)
    {
        throw ex;
    }
}

bool LineManager::eventFilter(unsigned int col, unsigned int line, char character)
{
    Line* ptr;
    int position = 0;

    try
    {
        ptr = m_lines.at(line);
        Field* type = ptr->searchAtCol(col);

        if( type != nullptr )
        {
            if( type->m_type == Title)
            {
                return false;
            }
#if __LOG_TEST__
            logFile.write("[DataMananager] Appending new character [%c] at [%d] in line [%d]", character, col, line);
#endif
            type->appendChar(character, col);

            for( unsigned int i = type->m_position + 1; i < ptr->m_fields.size(); i++ )
            {
                Field* o = ptr->m_fields.at(i);
                o->shiftRight();
            }

            return true;
        }
        else
        {
            position = (unsigned int)ptr->m_fields.size();
        }
    }
    catch(std::out_of_range& /*ex*/)
    {
#ifdef __LOG_TEST__
        logFile.write("[DataMananager][Create new line] Inserting new line [%d]", line);
#endif
        ptr = new Line;
        ptr->m_number = line;
    }

    Field* type = new Field(position, Text, col, col);
#ifdef __LOG_TEST__
    logFile.write("[DataMananager] Appending new character [%c] at [%d] in line [%d]", character, col, line);
#endif
    type->appendChar(character, col);
    ptr->m_fields.push_back(type);
    m_lines.insert(std::make_pair(line, ptr));
    return true;
}
