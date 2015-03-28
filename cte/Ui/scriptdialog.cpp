#include "scriptdialog.h"
#include "ui_scriptdialog.h"

#include <QAbstractListModel>

//Implementação interna da classe modelo de script
class ScriptModel : public QAbstractListModel
{
    // QAbstractItemModel interface
public:
    ScriptModel(QWidget* parent = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void setScripts(const std::vector<const Script*>& scripts);

private:
    const Script* scriptAt(int offset) const;
    std::vector<const Script*> m_scripts;
};

ScriptModel::ScriptModel(QWidget *parent) :
    QAbstractListModel(parent)
{

}

void ScriptModel::setScripts(const std::vector<const Script *>& scripts)
{
    m_scripts = scripts;
}

int ScriptModel::rowCount(const QModelIndex &parent) const
{
    return m_scripts.size();
}

const Script* ScriptModel::scriptAt(int offset) const
{
    return m_scripts.at(offset);
}

QVariant ScriptModel::data(const QModelIndex &index, int role) const
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
        const Script* script = scriptAt(index.row());
        return QString::fromStdString(script->Name());
    }

    return QVariant();
}

ScriptDialog::ScriptDialog(const std::vector<const Script*>& scripts, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScriptDialog)
{
    ui->setupUi(this);
    ScriptModel* model = new ScriptModel;
    model->setScripts(scripts);
    ui->scriptList->setModel(model);

    m_scripts = scripts;

    //slots
    connect(ui->btnCancel, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->btnLoad, SIGNAL(clicked(bool)), this, SLOT(load()));
}

ScriptDialog::~ScriptDialog()
{
    delete ui;
}

void ScriptDialog::load()
{
    emit accept();
}

const Script* ScriptDialog::selectedScript()
{
    std::cout << "current row" << ui->scriptList->selectionModel()->currentIndex().row() << std::endl;
    return m_scripts.at(ui->scriptList->selectionModel()->currentIndex().row());
}
