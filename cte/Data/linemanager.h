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

/**
 * @brief Enumeration of possible objects contained at line
 */
enum FieldType
{
    None = 0,
    Text,
    Name,
    Title,
    Relation,
    MaxFieldType
};

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
     * @brief default ctor
     */
    Field();
    /**
     * @brief Specialized ctor
     * @param pos the position of object
     * @param type the type of object
     * @param start the initial column
     * @param end the size of object
     */
    Field(int pos, FieldType type, unsigned int m_start, unsigned int m_end);
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
    /**
     * @brief shiftRight shift the string one position to right
     */
    void shiftRight();
    /**
     * @brief shiftLeft shift the string one position to left
     */
    void shiftLeft();
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
    Field* searchAtCol(unsigned int col);
    /**
     * @brief dump dump the lines
     * @return the lines dumped
     */
    std::string dump();
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
    unsigned int lineCount();

    //Operations
    /**
     * @brief addTitle Insert a title on current position
     * @param str the title
     * @param col the position of cursor
     * @param line the line of cursor
     */
    void addTitle(const char *str, unsigned int col, unsigned int line);
    /**
     * @brief addName Insert a name on current position
     * @param str the name
     * @param col the position of cursor
     * @param line the line of cursor
     */
    void addName(const char* str, unsigned int col, unsigned int line);
    /**
     * @brief removeCharacter Remove one character from object
     * @param col the position of cursor
     * @param line the line of cursor
     */
    void removeCharacter(unsigned int col, unsigned int line);
    /**
     * @brief dumpLine Dump one line of structure
     * @param line the line to be dumped
     * @return the line dumped
     */
    std::string dumpLine(unsigned int line);
    /**
     * @brief dumpLines Dump all lines of structure
     * @return the lines dumped
     */
    std::string dumpLines();
#ifdef __LOG_TEST__
    //Debug
    /**
     * @brief dump Dump the structure in log file
     */
    void dump();
#endif
private:
    /**
     * @brief Collection of lines
     */
    std::map<int, Line*> m_lines;
};

#endif // LINEMANAGER_H
