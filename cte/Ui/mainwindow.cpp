#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextCodec>
#include <QMessageBox>
#include <QSplitter>
#include <QTreeWidget>

//Dialogs
#include <QDialog>
#include <QInputDialog>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_editor(nullptr), m_proxy(nullptr), m_luaProcessor(nullptr)
{
    ui->setupUi(this);
    setupEditor();
    setCentralWidget(m_editor);

    connect(ui->act_goto, SIGNAL(triggered()), this, SLOT(goToLine()));
    connect(ui->act_installScripts, SIGNAL(triggered()), this, SLOT(installScripts()));
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
}

MainWindow::~MainWindow()
{
    if( m_editor )
        delete m_editor;
    if( m_proxy )
        delete m_proxy;
    if( m_luaProcessor )
        delete m_luaProcessor;
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    m_editor = new PlainEditor;
    m_editor->setFont(font);
    m_editor->installEventFilter(this);

    m_luaProcessor = new LuaProcessor;
    m_proxy = new EditorProxy(m_luaProcessor->state());
    Luna<EditorProxy>::Register(m_luaProcessor->state(), m_proxy);
    m_proxy->registerEditor(m_editor);
}

bool MainWindow::eventFilter(QObject * obj, QEvent * event)
{
    if( obj == m_editor )
    {
        //Update relevant information
        updateStatusBar();
        return m_editor->eventFilter(obj, event);
    }

    return QObject::eventFilter(obj, event);
}

void MainWindow::updateStatusBar()
{
    if( m_editor != nullptr )
    {
        QString status = QString(tr("Coluna: %1 Editável: %2 Tipo: %3")).
                arg(m_editor->columnNumber()).
                arg(m_editor->isEditable() ? "sim" : "não").
                arg(m_editor->objectType());
        ui->m_statusBar->showMessage(status);
    }
}

////////////////
//Slots

void MainWindow::goToLine()
{
    bool ok;
    int maxLine = this->m_editor->lineCount();
    QString str = QString("Informe a linha entre 1 e %1").arg(maxLine);
    int line = QInputDialog::getInt(this, "Ir para linha...", str, 1, 1, maxLine, 1, &ok);

    if( ok )
    {
        this->m_editor->moveToLine(line - 1);
    }
}

void MainWindow::installScripts()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Escolha o script a ser instalado...", "./scripts/", "*.lua");

    if(fileName.isEmpty() || fileName.isNull())
    {
        return;
    }

    //ToDo
    //Abre arquivo e joga o binário no banco
}
