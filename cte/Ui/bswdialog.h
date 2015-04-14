#ifndef BSWDIALOG_H
#define BSWDIALOG_H

#include <QDialog>

namespace Ui {
class BSWDialog;
}

class BSWDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BSWDialog(QWidget *parent = 0);
    ~BSWDialog();

private:
    bool exists(QString& name);

private slots:
    void createBSW();

private:
    Ui::BSWDialog *ui;
};

#endif // BSWDIALOG_H
