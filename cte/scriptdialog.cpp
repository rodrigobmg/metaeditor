#include "scriptdialog.h"
#include "ui_scriptdialog.h"

ScriptDialog::ScriptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScriptDialog)
{
    ui->setupUi(this);
}

ScriptDialog::~ScriptDialog()
{
    delete ui;
}
