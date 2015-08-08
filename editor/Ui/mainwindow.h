#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//QT
#include <QMainWindow>
#include <QPainter>
#include <QSplitter>
#include <QTreeView>
#include <QMdiArea>

//Editor
#include "../editor.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

protected:
    virtual bool eventFilter(QObject * obj, QEvent * event);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void paintEvent(QPaintEvent *);

private slots:
    void EditorDestroy();
    void Update();

private:
    //Ui
    Ui::MainWindow* ui;
    GUI::Editor* m_editor;
};

#endif // MAINWINDOW_H
