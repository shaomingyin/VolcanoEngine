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
    onSourceChanged(world_mesh->source());
    connect(world_mesh_, &World::Mesh::sourceChanged, this, &Mesh::onSourceChanged, Qt::QueuedConnection);
}

void Mesh::onSourceChanged(const QUrl& v) {
    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);
    Q_ASSERT(QOpenGLContext::currentContext()->thread()->isCurrentThread());

    vertex_buffer_.reset();
    index_buffer_.reset();

    auto content = world_mesh_->requestContent();
    if (content == nullptr) {
        return;
    }

    auto on_finished = [this, content] {
        vertex_buffer_.reset(context_.allocStaticVertexBuffer(128));
        index_buffer_.reset(context_.allocStaticIndexBuffer(128));
        // TODO
    };

    if (content->isFinished()) {
        on_finished();
    } else {
        connect(content, &QNetworkReply::finished, this, std::move(on_finished));
    }
}

QDataStream &operator<<(QDataStream& s, const Mesh& mesh) {
    return s;
}

QDataStream &operator>>(QDataStream& s, Mesh& mesh) {
    return s;
}

VOLCANO_GRAPHICS_END
