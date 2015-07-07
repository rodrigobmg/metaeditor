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
*   05/2015 1.3 Alteração na forma como se criam os campos no editor
*               para versões mais antigas buscar no git.
**/
#include <string>
#include <lua.hpp>
typedef int (*append)(lua_State*);

/**
 * @brief Interface IEditor
 */
class IEditor
{
public:
    /**
     * @brief currentLine get the current line to be edited
     * @return the current line
     * @version 1.1
     */
    virtual int currentLine() = 0;
    /**
     * @brief currentColumn get the current column of editor
     * @return the current column
     * @version 1.1
     */
    virtual int currentColumn() = 0;
    /**
     * @brief jumpLine jump one line in editor
     * @version 1.1
     */
    virtual void jumpLine() = 0;
    /**
     * @brief createField create a new field of a given type
     * @version 1.3
     */
    virtual void createField(int type, const std::string& text, std::string* fieldCallback) = 0;
    /**
    * @brief moveCursor moves the editor cursor many times as specified
    */
    virtual void moveCursor(int many) = 0;
};

#endif // IEDITOR_H
