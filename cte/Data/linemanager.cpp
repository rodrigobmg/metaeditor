#include "linemanager.h"

#include <exception>

#ifdef __LOG_TEST__
#include "../Utils/logfile.h"
static LogFile logFile("DataManager", "LogTest", true);
#endif

namespace util
{
    template<typename _Iterator1, typename _Iterator2>
    inline size_t IncUtf8StringIterator(_Iterator1& it, const _Iterator2& last)
    {
        if(it == last) return 0;
        unsigned char c;
        size_t res = 1;
        for(++it; last != it; ++it, ++res) {
            c = *it;
            if(!(c&0x80) || ((c&0xC0) == 0xC0)) break;
        }

        return res;
    }

    inline size_t Utf8StringSize(const std::string& str)
    {
        size_t res = 0;
        std::string::const_iterator it = str.begin();
        for(; it != str.end(); IncUtf8StringIterator(it, str.end()))
            res++;

        return res;
    }
}

Field::Field() :
    m_position(0), m_type(Text), m_start(0), m_end(0)
{
    //empty
}

Field::Field(int pos, FieldType type, unsigned int start) :
    m_position(pos), m_type(type), m_start(start), m_end(start)
{
    //empty
}

bool Field::atRange(unsigned int col)
{
    return col >= m_start && col <= m_end;
}

void Field::appendInfo(std::string c, unsigned int pos)
{
    std::cout << "Appending Info: " << c << std::endl;
    m_info.insert( (pos-m_start), c);
    m_end += util::Utf8StringSize(c) - 1;
    if( m_type != Title )
    {
        m_end += 1;
        m_fieldCallback->insert( (pos-m_start), c);
    }
}

void Field::appendChar(char c, unsigned int pos)
{
    std::cout << "Appending Char: " << c << std::endl;
    m_info.insert( (pos-m_start) , 1, c);
    m_end++;
    if( m_type != Title)
    {
        m_fieldCallback->insert((pos-m_start), 1, c);
    }
}

void Field::removeAt(unsigned int pos)
{
    m_info.erase( (pos-m_start), 1);
    m_end--;
    if( m_type != Title)
    {
        m_fieldCallback->erase( (pos-m_start), 1);
    }
}

void Field::print()
{
    std::cout << "======FIELD======" << std::endl;
    std::cout << "Field number: " << m_position << std::endl;
    std::cout << "Field type: " << m_type << std::endl;
    std::cout << "Field start: " << m_start << std::endl;
    std::cout << "Field end: " << m_end << std::endl;
    if( m_type != Title )
        std::cout << "Field callback: " << m_fieldCallback->c_str() << std::endl;
    std::cout << "======FIELD======" << std::endl;
}

Field* Line::getFieldByColumn(unsigned int col)
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

LineManager::LineManager()
{
    //empty
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

unsigned int LineManager::count()
{
    return (unsigned int)m_lines.size();
}

void LineManager::removeCharacter(unsigned int col, unsigned int line)
{
    try
    {
        Line* pLine = m_lines.at(line);

        if( pLine == nullptr )
        {
            return;
        }

        Field* pField = pLine->getFieldByColumn(col);

        if( pField != nullptr )
        {
            pField->removeAt(col - 1);
        }
    }
    catch(std::exception& ex)
    {

        std::cout << "removeCharacter" << std::endl;
        throw ex;
    }
}

bool LineManager::isEditable(unsigned int col, unsigned int line)
{
    if(!isValid(line))
    {
        return false;
    }

    try
    {
        Line* pLine = m_lines.at(line);

        if(pLine == nullptr)
        {
            return false;
        }

        Field* pField = pLine->getFieldByColumn(col);

        if(pField != nullptr)
        {
            if( pField->m_type == Title )
            {
                return false;
            }
        }

        return true;
    }
    catch(std::exception& ex)
    {

        std::cout << "isEditable" << std::endl;
        throw ex;
    }
}

std::string LineManager::type(unsigned int col, unsigned int line) throw(std::exception)
{
    if(!isValid(line))
    {
        return "None";
    }

    try
    {
        Line* pLine = m_lines.at(line);

        if( pLine == nullptr )
        {
            return "Text";
        }

        Field* pField = pLine->getFieldByColumn(col);

        if(pField != nullptr)
        {
            switch(pField->m_type)
            {
                case Title:
                    return "Title";
                case Text:
                    return "Text";
                case Name:
                    return "Name";
                case None:
                case MaxFieldType:
                    return "Error";
            }
        }

        return "Text";
    }
    catch(std::exception& ex)
    {

        std::cout << "type" << std::endl;
        throw ex;
    }
}

bool LineManager::eventFilter(unsigned int col, unsigned int line, char character)
{
    try
    {
        Line* pLine = m_lines.at(line);

        if(pLine == nullptr)
        {
            return false;
        }

        Field* pField = pLine->getFieldByColumn(col);

        if( pField != nullptr )
        {
            if( pField->m_type == Title)
            {
                return false;
            }

            pField->appendChar(character, col);
            return true;
        }
    }
    catch(std::exception& ex)
    {
        std::cout << "eventFilter" << std::endl;
        throw ex;
    }

    return false;
}

bool LineManager::isValid(unsigned int line)
{
    if(m_lines.empty())
    {
        return false;
    }

    int linecount = m_lines.size();

    if(line < linecount)
    {
        return true;
    }

    return false;
}

bool LineManager::isField(unsigned int line, unsigned int col)
{
    try
    {
        if( !isValid(line))
        {
            return false;
        }

        Line* pLine = m_lines.at(line);

        if(pLine != nullptr)
        {
            if( pLine->getFieldByColumn(col) != nullptr )
            {
                return true;
            }
        }
    }
    catch(std::exception& ex)
    {
        std::cout << "isField" << std::endl;
    }

    return false;
}

void LineManager::createField(FieldType type, const std::string &text, std::string* fieldCallback, unsigned int start, unsigned int line)
{
    Line* pLine = nullptr;

    if( isValid(line) )
    {
        pLine = m_lines.at(line);
    }
    else
    {
        pLine = new Line;
        pLine->m_number = line;
        m_lines.insert(std::make_pair(line, pLine));
    }

    Field* pField = new Field(pLine->m_fields.size(), type, start);

    if( type != Title )
    {
        fieldCallback->clear();
        pField->m_fieldCallback = fieldCallback;
    }

    pField->appendInfo(text, start);
    pField->print();

    pLine->m_fields.push_back(pField);
    std::cout << "Line " << line << " has " << pLine->m_fields.size() << " fields" << std::endl;
}

void LineManager::backspace(unsigned int line, unsigned int column)
{
}

void LineManager::getField(unsigned int line)
{

}


















