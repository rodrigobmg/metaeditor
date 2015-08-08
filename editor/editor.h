#ifndef EDITOR_H
#define EDITOR_H

//Qt
#include <QPlainTextEdit>

//Lua
#include "Lua/luna.h"

namespace GUI
{

//Classe encapsulada no arquivo editor.cpp
class Field;
extern const int Unlimited;

class Editor : public QPlainTextEdit
{
    Q_OBJECT

public:
    Editor();
    explicit Editor(const std::string luaScript, int objectId);
    ~Editor();

    //Desenha a estrutura de dados
    void Paint();

    //Herança
    virtual bool eventFilter(QObject *, QEvent *);

public:
    //funções lua
    int CreateField(lua_State* env);
    int NextLine(lua_State* env);
    int Indent(lua_State* env);

    //definições para uso na interface Luna
    static const char className[];
    static const Luna<Editor>::RegType Register[];

signals:
    void Repaint();

private:
    //Versão especializada para criação do campo de edição
    void CreateField(int objectValue, const std::string& text, int size = Unlimited);
    Field* GetField(unsigned int line, unsigned int column);
    Field* GetLastFieldOfLine(unsigned int line);
    bool Empty() const;

    //Funções de apoio ao editor de texto
    void UpdateCursor(int currentLine, int currentColumn);
    void JumpLine(int n = 1);

private:
    std::vector<Field*> m_fields;
    lua_State* m_luaState;

    //Iteradores - Utilizados somente para a execução
    //             do script Lua
    int m_currentLine;
    int m_currentColumn;

    //Objeto corrente que está sendo alterado pelo editor
    int m_currentObject;
};

}

#endif // EDITOR_H
