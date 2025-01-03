//
//
#ifndef VOLCANO_GRAPHICS_MESH_H
#define VOLCANO_GRAPHICS_MESH_H

#include <memory>

#include <QObject>
#include <QDataStream>
#include <QOpenGLVertexArrayObject>

#include <Volcano/World/Entity.h>
#include <Volcano/World/Mesh.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Buffer.h>
#include <Volcano/Graphics/Context.h>

VOLCANO_GRAPHICS_BEGIN

class Mesh: public QObject {
    Q_OBJECT

    friend QDataStream &operator<<(QDataStream&, const Mesh&);
    friend QDataStream &operator>>(QDataStream&, Mesh&);

public:
    Mesh(Context& context, World::Entity* world_entity, World::Mesh* world_mesh, QObject* parent = nullptr);

public:
    World::Entity* worldEntity() {
        return world_entity_;
    }

    const World::Entity* worldEntity() const {
        return world_entity_;
    }

    World::Mesh* worldMesh() {
        return world_mesh_;
    }

    const World::Mesh* worldMesh() const {
        return world_mesh_;
    }

private slots:
    void onSourceChanged(const QUrl& v);

private:
    Context& context_;
    World::Entity* world_entity_;
    World::Mesh* world_mesh_;
    QOpenGLVertexArrayObject vao_;
    std::unique_ptr<Buffer> vertex_buffer_;
    std::unique_ptr<Buffer> index_buffer_;
};

QDataStream &operator<<(QDataStream&, const Mesh&);
QDataStream &operator>>(QDataStream&, Mesh&);

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MESH_H
