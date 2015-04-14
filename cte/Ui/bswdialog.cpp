#include "bswdialog.h"
#include "ui_bswdialog.h"

#include <QMessageBox>

#include <core/DAO/databasemanager.h>
#include "../Objects/softwarebase.h"

BSWDialog::BSWDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BSWDialog)
{
    ui->setupUi(this);
    connect(ui->btnCreate, SIGNAL(clicked()), this, SLOT(createBSW()));
}

BSWDialog::~BSWDialog()
{
    delete ui;
}

bool BSWDialog::exists(QString &name)
{
    try
    {
        if(DatabaseManager::instance().read<SoftwareBase>("BSW", name.toStdString()) != nullptr)
        {
            return false;
        }

        return true;
    }
    catch(DatabaseException& ex)
    {
        QMessageBox::critical(this, "Erro de acesso ao banco de dados", ex.what());
    }

    return false;
}

void BSWDialog::createBSW()
{
    QString name = ui->lnBswName->text();

    if(exists(name))
    {
        QMessageBox::critical(this, "Erro", "A base de software informada já existe no banco de dados.");
        return;
    }

    try
    {
        SoftwareBase bsw(name.toStdString());
        if( !DatabaseManager::instance().create(bsw, "BSW") )
        {
            QMessageBox::critical(this, "Erro", "Falha ao criar base de software");
        }
    }
    catch(DatabaseException& ex)
    {
        QMessageBox::critical(this, "Erro de acesso ao banco de dados", ex.what());
    }
}
