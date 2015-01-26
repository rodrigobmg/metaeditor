#ifndef IEDITOR_H
#define IEDITOR_H

/**
* @modulo IEditor
* @autor  Rodrigo Monteiro 1313493
* @descrição Interface usada pelo @see{EditorProxy} para acesso a
*            funções do editor de textos
* @versão
*   12/2014 1.0 Módulo inicial
*   12/2014 1.1 Módulo finalizado
*   12/2014 1.2 Módulo reaberto para inserção de novas funcionalidades
**/

/**
 * @brief Interface IEditor
 */
class IEditor
{
public:
    /**
     * @brief currentLine get the current line to be edited
     * @return the current line
     */
    virtual int currentLine() = 0;
    /**
     * @brief currentColumn get the current column of editor
     * @return the current column
     */
    virtual int currentColumn() = 0;
    /**
     * @brief jumpLine jump one line in editor
     */
    virtual void jumpLine() = 0;
    /**
     * @brief addTitle insert a title at document
     * @version 1.2
     */
    virtual void addTitle(const char* str) = 0;
    /**
     * @brief addName insert a name at document
     * @version 1.2
     */
    virtual void addName(const char* str) = 0;
#ifdef __LOG_TEST__
    /**
     * @brief addName get number of lines
     * @version 1.2
     */
    virtual int structureLineCount() = 0;
    /**
     * @brief writeText method to write the text
     * @param str texto to be writed
     */
    virtual void writeText(const char* str) = 0;
#endif
};

#endif // IEDITOR_H
