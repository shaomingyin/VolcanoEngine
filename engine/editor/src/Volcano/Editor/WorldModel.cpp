//
//
#include <QFileInfo>

#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Editor/WorldModel.hpp>

VOLCANO_EDITOR_BEGIN

WorldModel::WorldModel(QObject *parent):
    QAbstractItemModel(parent),
    m_gameWorld(nullptr)
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
    return 1;
}

QVariant WorldModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    auto node = reinterpret_cast<Node *>(index.internalPointer());
    Q_ASSERT(node != nullptr);
    Q_ASSERT(node->object != nullptr);

    return QVariant(node->object->objectName());
}

bool WorldModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    return false;
}

Qt::ItemFlags WorldModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

bool WorldModel::hasChildren(const QModelIndex &parent) const
{
    return QAbstractItemModel::hasChildren(parent);

    if (!parent.isValid())
        return (m_gameWorld != nullptr);

    auto node = reinterpret_cast<Node *>(parent.internalPointer());
    Q_ASSERT(node != nullptr);
    Q_ASSERT(node->object != nullptr);

    auto gameWorld = qobject_cast<Game::World *>(node->object);
    if (gameWorld != nullptr) {
        Q_ASSERT(gameWorld == m_gameWorld);
        return (gameWorld->objectCount() > 0);
    }

    auto gameObject = qobject_cast<Game::Object *>(node->object);
    if (gameObject != nullptr) {
        auto gameEntity = qobject_cast<const Game::Entity *>(gameObject);
        if (gameEntity != nullptr)
            return (gameEntity->componentCount() > 0);
    }

    return false;
}

QVariant WorldModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QModelIndex WorldModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_ASSERT(column == 0);

    if (!parent.isValid()) {
        if (m_gameWorld != nullptr)
            return createIndex(row, column, new Node(parent, m_gameWorld));
        return QModelIndex();
    }

    auto node = reinterpret_cast<const Node *>(parent.constInternalPointer());
    Q_ASSERT(node != nullptr);
    Q_ASSERT(node->object != nullptr);

    auto gameWorld = qobject_cast<Game::World *>(node->object);
    if (gameWorld != nullptr) {
        Q_ASSERT(gameWorld == m_gameWorld);
        if (row < gameWorld->objectCount())
            return createIndex(row, column, new Node(parent, gameWorld->objectAt(row)));
        return QModelIndex();
    }

    auto gameObject = qobject_cast<Game::Object *>(node->object);
    if (gameObject != nullptr) {
        auto gameEntity = qobject_cast<Game::Entity *>(gameObject);
        if (gameEntity != nullptr && row < gameEntity->componentCount())
            return createIndex(row, column, new Node(parent, gameEntity->componentAt(row)));
        return QModelIndex();
    }

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
    if (!index.isValid())
        return QModelIndex();

    auto node = reinterpret_cast<Node *>(index.internalPointer());
    Q_ASSERT(node != nullptr);
    Q_ASSERT(node->object != nullptr);

    return node->parent;
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
    if (!parent.isValid())
        return (m_gameWorld != nullptr ? 1 : 0);

    if (parent.column() > 0)
        return 0;

    auto node = reinterpret_cast<Node *>(parent.internalPointer());
    Q_ASSERT(node != nullptr);
    Q_ASSERT(node->object != nullptr);

    auto gameWorld = qobject_cast<Game::World *>(node->object);
    if (gameWorld != nullptr) {
        Q_ASSERT(gameWorld == m_gameWorld);
        return gameWorld->objectCount();
    }

    auto gameObject = qobject_cast<Game::Object *>(node->object);
    if (gameObject != nullptr) {
        auto gameEntity = qobject_cast<Game::Entity *>(gameObject);
        if (gameEntity != nullptr)
            return gameEntity->componentCount();
    }

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
    if (m_gameWorld == gameWorld)
        return;

    beginResetModel();

    // TODO

    m_gameWorld = gameWorld;

    endResetModel();

    emit gameWorldChanged(gameWorld);
}

VOLCANO_EDITOR_END
