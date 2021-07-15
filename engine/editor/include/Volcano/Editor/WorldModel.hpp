//
//
#ifndef VOLCANO_EDITOR_WORLDMODEL_HPP
#define VOLCANO_EDITOR_WORLDMODEL_HPP

#include <QDir>
#include <QString>
#include <QModelIndex>
#include <QAbstractItemModel>

#include <Volcano/Game/World.hpp>
#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class WorldModel: public QAbstractItemModel {
    Q_OBJECT

public:
    WorldModel(QObject *parent = nullptr);
    ~WorldModel(void) override;

public:
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool canFetchMore(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool hasChildren(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    QStringList mimeTypes(void) const override;
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;
    QModelIndex parent(const QModelIndex &index) const override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    QHash<int, QByteArray> roleNames(void) const override;
    int rowCount(const QModelIndex &parent) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) override;
    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;
    QModelIndex sibling(int row, int column, const QModelIndex &index) const override;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;
    Game::World *gameWorld(void);
    void setGameWorld(Game::World *gameWorld);

public:
    bool init(const QString &path);
    bool load(const QString &path);
    const QDir &dir(void) const;

signals:
    void gameWorldChanged(Game::World *gameWorld);

protected:
    struct Node {
        Node(const QModelIndex &p, QObject *o):
            parent(p),
            object(o)
        {
        }

        QModelIndex parent;
        QObject *object;
    };

private:
    Game::World *m_gameWorld;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_WORLDMODEL_HPP
