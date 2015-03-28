#ifndef SCRIPTDIALOG_H
#define SCRIPTDIALOG_H

#include <QDialog>
#include <vector>
#include "../Objects/script.h"

namespace Ui {
class ScriptDialog;
}

class ScriptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptDialog(const std::vector<const Script*>& scripts, QWidget *parent = 0);
    ~ScriptDialog();

    const Script* selectedScript();

private slots:
    void load();

private:
    Ui::ScriptDialog *ui;
    std::vector<const Script*> m_scripts;
};

#endif // SCRIPTDIALOG_H
