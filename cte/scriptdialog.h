#ifndef SCRIPTDIALOG_H
#define SCRIPTDIALOG_H

#include <QDialog>

namespace Ui {
class ScriptDialog;
}

class ScriptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptDialog(QWidget *parent = 0);
    ~ScriptDialog();

private:
    Ui::ScriptDialog *ui;
};

#endif // SCRIPTDIALOG_H
