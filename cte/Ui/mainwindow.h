#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
* modulo MainWindow
* autor  Rodrigo Monteiro 1313493
* descrição Módulo responsável gerenciar a janela principal da aplicação
* versão
*   12/2014 1.0 Módulo inicial
*   12/2014 1.1 Módulo finalizado
**/

//QT
#include <QMainWindow>
#include <QPainter>
#include <QSplitter>
#include <QTreeView>
#include <QMdiArea>

//CoreLib
#include <Proxies/editorproxy.h>
#include <LuaWrapper/luaprocessor.h>

//CTE
#include "../plaineditor.h"
#include "Objects/softwarebase.h"

namespace Ui {
class MainWindow;
class BswDialog;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setupEditor();
    void updateStatusBar();
    void setupMainWindow();
    void loadScript(const std::string& scr);

protected:
    virtual bool eventFilter(QObject * obj, QEvent * event);
    /**
     * @brief mousePressEvent Filter the mouse events that happens in editor
     * @param e The mouse event
     */
    //virtual void mousePressEvent(QMouseEvent *e);

private slots:
    //Scripts
    void installScripts();
    void loadScript();
    void loadAllScripts();

    //BSW
    void createBSW();
    void loadBSW();
    void initBSW();

    //Editor
    //void openNewEditor();
    void goToLine();

private:
    //Ui
    Ui::MainWindow* ui;
    QSplitter m_splitter;
    QTreeView m_treeview;
    QMdiArea m_mdiarea;

    //Utils
    std::unique_ptr<PlainEditor>    m_editor;
    std::unique_ptr<EditorProxy>    m_proxy;

    /**
     * @brief m_luaProcessor Processador Lua de uso geral da aplicação
     */
    std::unique_ptr<LuaProcessor>   m_luaProcessor;
    /**
     * @brief m_curBSW Base de software aberta
     */
    std::unique_ptr<SoftwareBase>   m_curBSW;
};//5487

#endif // MAINWINDOW_H
