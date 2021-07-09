//
//
#include <QFileInfo>

#include <Volcano/Editor/WorldModel.hpp>

VOLCANO_EDITOR_BEGIN

WorldModel::WorldModel(QObject *parent):
    QAbstractItemModel(parent)
{
}

WorldModel::~WorldModel(void)
{
}

bool WorldModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    return QAbstractItemModel::canDropMimeData(data, action, row, column, parent);
}

bool WorldModel::canFetchMore(const QModelIndex &parent) const
{
    return QAbstractItemModel::canFetchMore(parent);
}

int WorldModel::columnCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant WorldModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

bool WorldModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    return false;
}

Qt::ItemFlags WorldModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

bool WorldModel::hasChildren(const QModelIndex &parent = QModelIndex()) const
{
    return false;
}

QVariant WorldModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QModelIndex WorldModel::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QMimeData *WorldModel::mimeData(const QModelIndexList &indexes) const
{
    return QAbstractItemModel::mimeData(indexes);
}

QStringList WorldModel::mimeTypes(void) const
{
    return QAbstractItemModel::mimeTypes();
}

bool WorldModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    return QAbstractItemModel::moveRows(sourceParent, sourceRow, count, destinationParent, destinationChild);
}

QModelIndex WorldModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

bool WorldModel::removeRows(int row, int count, const QModelIndex &parent)
{
    return QAbstractItemModel::removeRows(row, count, parent);
}

QHash<int, QByteArray> WorldModel::roleNames(void) const
{
    return QAbstractItemModel::roleNames();
}

int WorldModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

bool WorldModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

bool WorldModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    return false;
}

bool WorldModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
    return false;
}

QModelIndex WorldModel::sibling(int row, int column, const QModelIndex &index) const
{
    return QAbstractItemModel::sibling(row, column, index);
}

Qt::DropActions WorldModel::supportedDragActions() const
{
    return supportedDragActions();
}

Qt::DropActions WorldModel::supportedDropActions() const
{
    return supportedDropActions();
}

Game::World *WorldModel::gameWorld(void)
{
    return m_gameWorld;
}

void WorldModel::setGameWorld(Game::World *gameWorld)
{
    m_gameWorld = gameWorld;
}

VOLCANO_EDITOR_END
