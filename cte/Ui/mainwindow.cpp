#include "mainwindow.h"
#include "ui_mainwindow.h"

//Utils
#include <QTextCodec>
#include <QMessageBox>

//Widgets
#include <QSplitter>
#include <QTreeWidget>

//Dialogs
#include <QDialog>
#include <QInputDialog>
#include <QFileDialog>

//Painter
#include <QPainter>

//Objects
#include <DAO/databasemanager.h>
#include "../Objects/script.h"

//Dialogs
#include "scriptdialog.h"
#include "bswdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_editor(nullptr), m_proxy(nullptr), m_luaProcessor(nullptr)
{
    ui->setupUi(this);
    setupMainWindow();

    PlainEditor* editor = setupEditor();
    connect(editor, SIGNAL(openNewEditor()), this, SLOT(openNewEditor()));

    m_mdiarea.addSubWindow(editor);
    m_mdiarea.setFocusPolicy(Qt::NoFocus);

    //m_splitter.addWidget(m_editor);
    setCentralWidget(&m_splitter);

    //Slots de gerais
    connect(ui->act_goto, SIGNAL(triggered()), this, SLOT(goToLine()));
    connect(ui->act_exit, SIGNAL(triggered()), this, SLOT(close()));
    //Slots da base de conhecimento
    connect(ui->act_installScripts, SIGNAL(triggered()), this, SLOT(installScripts()));
    connect(ui->act_loadScript, SIGNAL(triggered()), this, SLOT(loadScript()));
    connect(ui->act_loadAllScripts, SIGNAL(triggered()), this, SLOT(loadAllScripts()));
    //Slots da base de software
    connect(ui->act_createBSW, SIGNAL(triggered()), this, SLOT(createBSW()));
    connect(ui->act_loadBSW, SIGNAL(triggered()), this, SLOT(loadBSW()));
}

MainWindow::~MainWindow()
{
    if( m_editor )
    {
        delete m_editor;
        m_editor = nullptr;
    }

    //if( m_proxy )
        //delete m_proxy;

    if( m_luaProcessor )
    {
        //Luna<EditorProxy>::gc_obj(m_luaProcessor->state());
        delete m_luaProcessor;
    }
}

PlainEditor* MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    PlainEditor* editor = new PlainEditor;
    editor->setFont(font);
    editor->installEventFilter(this);

    m_proxy->registerEditor(editor);

    return editor;
}

bool MainWindow::eventFilter(QObject * obj, QEvent * event)
{
    /*if( obj == m_editor )
    {
        //Update relevant information

        return m_editor->eventFilter(this, event);
    }*/
    updateStatusBar();
    return QObject::eventFilter(obj, event);
}
/*
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
    //QObject::mousePressEvent(e);
}
*/

void MainWindow::updateStatusBar()
{
    if( m_editor != nullptr )
    {
        QString status = QString(tr("Coluna: %1 Editável: %2 Tipo: %3")).
                arg(m_editor->columnNumber()).
                arg(m_editor->isEditable() ? "sim" : "não").
                arg(m_editor->objectType());
        std::cout << status.toStdString() << std::endl;
        ui->m_statusBar->showMessage(status);
    }
}

void MainWindow::setupMainWindow()
{
    //Configure the split between editors and treeview
    m_splitter.setOrientation(Qt::Horizontal);
    m_splitter.setStretchFactor(1, 3);

    //Mdi configurations
    m_mdiarea.setViewMode(QMdiArea::TabbedView);
    m_mdiarea.setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiarea.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiarea.setTabsClosable(true);

    //Insert widgets at splitter
    m_splitter.addWidget(&m_treeview);
    m_splitter.addWidget(&m_mdiarea);

    //Setup Lua
    m_luaProcessor = new LuaProcessor;
    m_proxy = new EditorProxy(m_luaProcessor->state());
    Luna<EditorProxy>::Register(m_luaProcessor->state(), m_proxy);
}

////////////////
//Slots

void MainWindow::openNewEditor()
{
    PlainEditor* editor = setupEditor();
    connect(editor, SIGNAL(openNewEditor()), this, SLOT(openNewEditor()));
    this->m_mdiarea.addSubWindow(editor);
}

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
    std::string fullpath = QFileDialog::getOpenFileName(this, "Escolha o script a ser instalado...", "./scripts/", "*.lua").toStdString();

    if(fullpath.empty())
    {
        return;
    }

    try
    {
        int slash = fullpath.find_last_of("/") + 1;
        std::string filename = fullpath.substr(slash, fullpath.size());

        Script* script = DatabaseManager::instance().read<Script>("BCO", filename);

        if(script != nullptr)
        {
            QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                      "Atenção",
                                                                      "Script informado já existe no banco de dados. Deseja atualizá-lo?",
                                                                      QMessageBox::Yes|QMessageBox::No);
            if(reply == QMessageBox::No)
            {
                delete script;
                return;
            }
        }

        if( m_luaProcessor->compileScript(fullpath) == true )
        {
            m_luaProcessor->doBuffer();

            if( script != nullptr )
            {
                script->setBinaryData((char*)m_luaProcessor->data(), m_luaProcessor->dataSize());

                if( !DatabaseManager::instance().update(*script, "BCO") )
                {
                    QMessageBox::information(this, "Erro", "Não foi possível atualizar o script no banco de dados.");
                    return;
                }

                QMessageBox::information(this, "Sucesso", "Script atualizado com sucesso.");
            }
            else
            {
                script = new Script(filename, (char*)m_luaProcessor->data(), m_luaProcessor->dataSize());

                if( !DatabaseManager::instance().create(*script, "BCO") )
                {
                    QMessageBox::information(this, "Erro", "Não foi possível salvar script no banco de dados.");
                    return;
                }

                QMessageBox::information(this, "Sucesso", "Script instalado com sucesso.");
            }
        }
        else
        {
            QMessageBox::critical(this, "Erro", "Erro ao compilar script.");
        }
    }
    catch(std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void MainWindow::loadScript(const std::string &scr)
{
    const Script* script = DatabaseManager::instance().read<Script>("BCO", scr);

    if(script != nullptr)
    {
        m_luaProcessor->doBuffer(script->binaryData(), script->size());
    }
}

void MainWindow::loadScript()
{
    try
    {
        std::vector<const Script*> scripts = DatabaseManager::instance().findAll<Script>("BCO");

        if(scripts.empty())
        {
            QMessageBox::warning(this, "Erro", "Não existem scripts na base de conhecimento");
            return;
        }

        ScriptDialog dialog(scripts, this);

        if(dialog.exec() == QDialog::Accepted)
        {
            const Script* script = dialog.selectedScript();
            m_luaProcessor->doBuffer(script->binaryData(), script->size());
        }
    }
    catch(std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void MainWindow::loadAllScripts()
{
    try
    {
        std::vector<const Script*> scripts = DatabaseManager::instance().findAll<Script>("BCO");

        if(scripts.empty())
        {
            QMessageBox::warning(this, "Erro", "Não existem scripts na base de conhecimento");
            return;
        }

        for(auto script : scripts)
        {
            m_luaProcessor->doBuffer(script->binaryData(), script->size());
        }
    }
    catch(std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void MainWindow::createBSW()
{
    BSWDialog dialog;
    dialog.exec();
}
