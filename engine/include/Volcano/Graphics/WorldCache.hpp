//
//
#ifndef VOLCANO_GRAPHICS_WORLDCACHE_HPP
#define VOLCANO_GRAPHICS_WORLDCACHE_HPP

#include <QObject>
#include <QVector3D>

#include <Volcano/World.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class WorldCache: public QObject {
    Q_OBJECT
    Q_PROPERTY(World *world READ world WRITE setWorld NOTIFY worldChanged)

public:
    WorldCache(QObject *parent = nullptr);
    ~WorldCache(void) override;

public:
    World *world(void);
    void setWorld(World *world);
    void update(const QVector3D &position);

signals:
    void worldChanged(World *world);

private:
    World *m_world;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_WORLDCACHE_HPP
