#ifndef LINEMANAGER_H
#define LINEMANAGER_H

/*
* modulo LineManager
* autor  Rodrigo Monteiro 1313493
* descrição Módulo responsável pela gerência da estrutura de dados do editor de textos
* versão
*   12/2014 1.0 Módulo inicial
*   12/2014 1.1 Módulo finalizado
**/

#include <map>
#include <vector>
#include <iostream>
#include <QObject>
#include <lua.hpp>

/**
 * @brief Enumeration of possible objects contained at line
 */
enum FieldType
{
    None = 0,
    Text,
    Name,
    Title,
    MaxFieldType
};


/**
 *
 * Título 1: campo1
 *
 *
 *
 *
 *
 */

/**
 * @brief Representation of an object.
 * The object has a position at line and one type
 */
struct Field
{
    /**
     * @brief object position at line
     */
    int m_position;
    /**
     * @brief type of object
     */
    FieldType m_type;
    /**
     * @brief the column of line which object starts
     */
    unsigned int m_start;
    /**
     * @brief It's the start + size of info
     */
    unsigned int m_end;
    /**
     * @brief The string representing the information contained at object
     */
    std::string m_info;
    /**
     * @brief m_fieldCallback
     */
    std::string* m_fieldCallback;

    /**
     * @brief default ctor
     */
    Field();
    /**
     * @brief Specialized ctor
     * @param pos the position of object
     * @param type the type of object
     * @param start the initial column
     */
    Field(int pos, FieldType type, unsigned int m_start);
    /**
     * @brief atRange Check if the text cursor is inside of object
     * @param col the column
     * @return true if it's inside of object
     */
    bool atRange(unsigned int col);
    /**
     * @brief appendInfo Append a string at object
     * @param c the string
     * @param pos the position inside string
     */
    void appendInfo(std::string c, unsigned int pos);
    /**
     * @brief appendChar Append a character at object
     * @param c the character
     * @param pos the position inside string
     */
    void appendChar(char c, unsigned int pos);
    /**
     * @brief removeAt Remove a single character of string
     * @param pos the position where character will be removed
     */
    void removeAt(unsigned int pos);

    void print();
};

/**
 * @brief Representation of one line
 */
struct Line
{
    /**
     * @brief The line number
     */
    unsigned int m_number;
    /**
     * @brief Vector of objects contained at line
     */
    std::vector<Field*> m_fields;

    /**
     * @brief searchAtCol search the object for the given column
     * @param col the column
     * @return the object if it exists, otherwise returns nullptr
     */
    Field* getFieldByColumn(unsigned int col);
};

const int Unlimited = -1;

inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}


inline std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

class FieldRC
{
public:

    FieldRC(int startLine, int startCol, int size = Unlimited) :
        m_startLine(startLine), m_endLine(startLine),
        m_startCol(startCol), m_endCol(startCol), m_size(size)
    {
        if( size != Unlimited )
        {
            m_endCol = startCol + size;
        }
    }

    bool append(char c)
    {
        if( m_size != Unlimited && (m_characters.size() + 1) > m_size )
        {
            return false;
        }

        m_characters.push_back(c);
        m_endCol++;
    }

    bool itsMe(int line)
    {
        if(line >= m_startLine && line <= m_endLine)
        {
            return true;
        }
    }

    /**
     * ToDo: Pensar melhor nessa função
     * @brief convertColumnByLine
     * @param line
     * @param column
     * @return
     */
    int convertColumnByLine(int line, int column)
    {
        std::vector<std::string> effectiveLines = split(m_characters, '\n');
        line = line - m_startLine;
        std::string characters = effectiveLines.at(line);
        return characters.at(column);
    }

private:
    int m_startLine; //Linha inicial
    int m_endLine; // Linha final, caso não tenha enter na string deve ser igual a linha inicial
    int m_startCol; // Coluna inicial na linha inicial
    int m_endCol;// Caso o tamanho máximo de caracteres seja diferente de -1, o tamanho vai ser a coluna de início + o tamanho
    int m_size; // Tamanho máximo de caracteres, caso não tenha limites deve ser -1
    std::string m_characters; //Caracteres
};

/**
 * @brief Manager of datastructure
 */
class LineManager
{
public:
    /**
     * @brief DataMan ctor
     */
    explicit LineManager();
    /**
     * @brief ~DataMan dtor
     */
    ~LineManager();

    //Utils
    /**
     * @brief eventFilter it filter the pressed keys at PlainEditor
     * @param col the position of cursor
     * @param line the line of cursor
     * @param character the key which was pressed
     * @return true if all ok, false otherwise
     */
    bool eventFilter(unsigned int col, unsigned int line, char character);
    /**
     * @brief isEditable Check if the object in current position is editable
     * @param col the position of cursor
     * @param line the line of cursor
     * @return true if it's not a Title
     */
    bool isEditable(unsigned int col, unsigned int line);
    /**
     * @brief type Returns the type of object
     * @param col the position of cursor
     * @param line the line of cursor
     * @return the string containing the name of type
     */
    std::string type(unsigned int col, unsigned int line) throw(std::exception);
    /**
     * @brief lineCount
     * @return the current number of lines
     */
    unsigned int count();
    /**
     * @brief removeCharacter Remove one character from object
     * @param col the position of cursor
     * @param line the line of cursor
     */
    void removeCharacter(unsigned int col, unsigned int line);

    //////////////////////
    ///Teste
    void createField(FieldType type, const std::string& text, std::string* fieldCallback, unsigned int start, unsigned int line);

    bool isField(unsigned int line, unsigned int col);

    bool isValid(unsigned int line);

    void backspace(unsigned int line, unsigned int column);
    void enter(unsigned int line, unsigned int column);
    void del(unsigned int line, unsigned int column);
    void tab(unsigned int line, unsigned int column);

private:
    Field* getField(unsigned int line);

private:
    /**
     * @brief Collection of lines
     */
    std::map<int, Line*> m_lines;
    /**
     * @brief conjunto de campos
     */
    std::map<int, Field**> m_fields;
};

#endif // LINEMANAGER_H
