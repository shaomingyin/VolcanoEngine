//
//
#include <Volcano/Editor/Project.hpp>

VOLCANO_EDITOR_BEGIN

Project::Project(QObject *parent):
    QAbstractItemModel(parent)
{
}

Project::~Project(void)
{
}

bool Project::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    return QAbstractItemModel::canDropMimeData(data, action, row, column, parent);
}

bool Project::canFetchMore(const QModelIndex &parent) const
{
    return QAbstractItemModel::canFetchMore(parent);
}

int Project::columnCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant Project::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

bool Project::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    return false;
}

Qt::ItemFlags Project::flags(const QModelIndex &index) const
{
    return Qt::NoItemFlags;
}

bool Project::hasChildren(const QModelIndex &parent = QModelIndex()) const
{
    return false;
}

QVariant Project::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QModelIndex Project::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QMimeData *Project::mimeData(const QModelIndexList &indexes) const
{
    return QAbstractItemModel::mimeData(indexes);
}

QStringList Project::mimeTypes(void) const
{
    return QAbstractItemModel::mimeTypes();
}

bool Project::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    return QAbstractItemModel::moveRows(sourceParent, sourceRow, count, destinationParent, destinationChild);
}

QModelIndex Project::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

bool Project::removeRows(int row, int count, const QModelIndex &parent)
{
    return QAbstractItemModel::removeRows(row, count, parent);
}

QHash<int, QByteArray> Project::roleNames(void) const
{
    return QAbstractItemModel::roleNames();
}

int Project::rowCount(const QModelIndex &parent) const
{
    return 0;
}

bool Project::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

bool Project::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    return false;
}

bool Project::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
    return false;
}

QModelIndex Project::sibling(int row, int column, const QModelIndex &index) const
{
    return QAbstractItemModel::sibling(row, column, index);
}

Qt::DropActions Project::supportedDragActions() const
{
    return supportedDragActions();
}

Qt::DropActions Project::supportedDropActions() const
{
    return supportedDropActions();
}

VOLCANO_EDITOR_END
