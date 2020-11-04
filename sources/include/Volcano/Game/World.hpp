//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QObject>
#include <QQmlListProperty>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class World: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    const QString &name(void) const;
    void setName(const QString &v);
    void update(float elapsed);

signals:
    void nameChanged(const QString &name);

private:
    QString m_name;
};

VOLCANO_INLINE const QString &World::name(void) const
{
    return m_name;
}

VOLCANO_INLINE void World::setName(const QString &v)
{
    if (m_name != v)
    {
        m_name = v;
        nameChanged(v);
    }
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
