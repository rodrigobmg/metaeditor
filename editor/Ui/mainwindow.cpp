#include "mainwindow.h"
#include "ui_mainwindow.h"

//Utils
#include <QTextCodec>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_editor = new GUI::Editor("/Users/Rodrigo/Desktop/scripts/teste.lua", 1);

    setCentralWidget(m_editor);

    //Filtro de eventos do editor -- delegando da janela principal para o editor
    m_editor->installEventFilter(this);

    //Conexões entre objetos
    //connect(&m_editor, &GUI::Editor::Destroy, this, &MainWindow::EditorDestroy);
    QObject::connect(m_editor, &GUI::Editor::Repaint, this, &MainWindow::Update);
}

MainWindow::~MainWindow()
{
    m_editor->removeEventFilter(this);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    return m_editor->eventFilter(obj, event);
}

void MainWindow::mousePressEvent(QMouseEvent * e)
{
    if(e->button() == Qt::LeftButton)
    {
        if(e->modifiers() == Qt::ControlModifier)
        {
            QMessageBox::information(this, "Teste", "Control + Click", QMessageBox::Ok);
        }
    }

    QMainWindow::mousePressEvent(e);
}

void MainWindow::paintEvent(QPaintEvent *pv)
{
    m_editor->setUpdatesEnabled(false);
    m_editor->Paint();
    qApp->processEvents();
    m_editor->setUpdatesEnabled(true);
}

////////////////
//Slots
void MainWindow::EditorDestroy()
{
    m_editor->removeEventFilter(this);
}

void MainWindow::Update()
{
    this->update();
}
