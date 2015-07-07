#ifndef LISTBSW_H
#define LISTBSW_H

#include <QDialog>
#include "../Objects/softwarebase.h"

namespace Ui {
class ListBSW;
}

class ListBSW : public QDialog
{
    Q_OBJECT

public:
    explicit ListBSW(const std::vector<const SoftwareBase*>& bsws, QWidget *parent = 0);
    ~ListBSW();

    const SoftwareBase* selectedBSW();

private slots:
    void open();

private:
    std::vector<const SoftwareBase*> m_bsw;
    Ui::ListBSW *ui;
};

#endif // LISTBSW_H
