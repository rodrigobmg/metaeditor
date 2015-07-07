#include "listbsw.h"
#include "ui_listbsw.h"

#include <QAbstractListModel>

//Implementação interna da classe modelo da base de software
class BSWModel : public QAbstractListModel
{
    // QAbstractItemModel interface
public:
    BSWModel(QWidget* parent = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void setBSW(const std::vector<const SoftwareBase*>& bsws);

private:
    const SoftwareBase* BSWAt(int offset) const;
    std::vector<const SoftwareBase*> m_bsw;
};

BSWModel::BSWModel(QWidget *parent) :
    QAbstractListModel(parent)
{

}

void BSWModel::setBSW(const std::vector<const SoftwareBase*>& bsws)
{
    m_bsw = bsws;
}

int BSWModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_bsw.size();
}

const SoftwareBase* BSWModel::BSWAt(int offset) const
{
    return m_bsw.at(offset);
}

QVariant BSWModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole)
    {
        const SoftwareBase* bsw = BSWAt(index.row());
        return QString::fromStdString(bsw->Name());
    }

    return QVariant();
}

ListBSW::ListBSW(const std::vector<const SoftwareBase *> &bsws, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListBSW)
{
    ui->setupUi(this);

    BSWModel* model = new BSWModel;
    model->setBSW(bsws);
    ui->lsBswList->setModel(model);

    m_bsw = bsws;

    //slots
    connect(ui->btnCancel, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->btnOpen, SIGNAL(clicked(bool)), this, SLOT(open()));
}

ListBSW::~ListBSW()
{
    delete ui;
}

void ListBSW::open()
{
    emit accept();
}

const SoftwareBase* ListBSW::selectedBSW()
{
    int index = ui->lsBswList->selectionModel()->currentIndex().row();
    return m_bsw.at(index);
}
