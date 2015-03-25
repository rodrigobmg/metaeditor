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
    void setupEditor();
    void updateStatusBar();

protected:
    virtual bool eventFilter(QObject * obj, QEvent * event);

private slots:
    void goToLine();
    void installScripts();

private:
    Ui::MainWindow* ui;
    PlainEditor*    m_editor;
    EditorProxy*    m_proxy;
    LuaProcessor*   m_luaProcessor;
};

#endif // MAINWINDOW_H
