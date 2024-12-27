//
//
#include <QOpenGLContext>

#include <Volcano/Graphics/Mesh.h>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(Context& context, World::Entity* world_entity, World::Mesh* world_mesh, QObject* parent)
    : QObject(parent)
    , context_(context)
    , world_entity_(world_entity)
    , world_mesh_(world_mesh) {
    Q_ASSERT(world_entity_ != nullptr);
    Q_ASSERT(world_mesh_ != nullptr);
    connect(world_mesh_, &World::Mesh::sourceChanged, this, &Mesh::onSourceChanged, Qt::QueuedConnection);
}

void Mesh::onSourceChanged(const QUrl& v) {
    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);

    vertex_buffer_.reset();
    index_buffer_.reset();

    // TODO
}

QDataStream &operator<<(QDataStream& s, const Mesh& mesh) {
    return s;
}

QDataStream &operator>>(QDataStream& s, Mesh& mesh) {
    return s;
}

VOLCANO_GRAPHICS_END
