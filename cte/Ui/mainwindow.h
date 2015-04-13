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
    PlainEditor *setupEditor();
    void updateStatusBar();
    void setupMainWindow();

protected:
    virtual bool eventFilter(QObject * obj, QEvent * event);
    /**
     * @brief mousePressEvent Filter the mouse events that happens in editor
     * @param e The mouse event
     */
    //virtual void mousePressEvent(QMouseEvent *e);

private slots:
    void goToLine();
    void installScripts();
    void loadScript();
    void loadAllScripts();

    //Editor
    void openNewEditor();

private:
    //Ui
    Ui::MainWindow* ui;
    QSplitter m_splitter;
    QTreeView m_treeview;
    QMdiArea m_mdiarea;

    //Utils
    PlainEditor*    m_editor;
    EditorProxy*    m_proxy;
    LuaProcessor*   m_luaProcessor;
};

#endif // MAINWINDOW_H
