#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
* @modulo MainWindow
* @autor  Rodrigo Monteiro 1313493
* @descrição Módulo responsável gerenciar a janela principal da aplicação
* @versão
*   12/2014 1.0 Módulo inicial
*   12/2014 1.1 Módulo finalizado
**/

#include <QMainWindow>
#include "../plaineditor.h"
#include <Proxies/editorproxy.h>
#include <LuaWrapper/luaprocessor.h>

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
#ifdef __LOG_TEST__
    void runTests();
#endif
private:
    Ui::MainWindow *ui;
    PlainEditor* m_editor;
    EditorProxy* m_proxy;
    LuaProcessor* m_luaProcessor;
};

#endif // MAINWINDOW_H
