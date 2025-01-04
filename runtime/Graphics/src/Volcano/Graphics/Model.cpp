//
//
#include <QScopeGuard>
#include <QDataStream>
#include <QOpenGLContext>

#include <Volcano/Graphics/Model.h>

VOLCANO_GRAPHICS_BEGIN

Model::Model(Context& context, World::Entity* world_entity, World::Model* world_model, QObject* parent)
    : QObject(parent)
    , context_(context)
    , world_entity_(world_entity)
    , world_model_(world_model) {
    Q_ASSERT(world_entity_ != nullptr);
    Q_ASSERT(world_model_ != nullptr);
    onSourceChanged(world_model->source());
    connect(world_model_, &World::Model::sourceChanged, this, &Model::onSourceChanged, Qt::QueuedConnection);
}

void Model::onSourceChanged(const QUrl& v) {
    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);
    Q_ASSERT(QOpenGLContext::currentContext()->thread()->isCurrentThread());

    if (loading_task_.isRunning()) {
        loading_task_.cancel();
        loading_task_.waitForFinished();
    }

    vertex_buffers_.clear();
    index_buffers_.clear();

    if (!v.isValid()) {
        return;
    }

    auto content = world_model_->requestContent();
    if (content == nullptr) {
        return;
    }

    auto on_finished = [this, content] {
        QPromise<void> promise;
        loading_task_ = promise.future();
        auto loading_thread = QThread::create([this, content, promise = std::move(promise)]() mutable {
            auto guard = qScopeGuard([content, &promise] {
                delete content;
                promise.finish();
            });
            load(content, promise);
        });
    };

    if (content->isFinished()) {
        on_finished();
    } else {
        connect(content, &QNetworkReply::finished, this, std::move(on_finished));
    }
}

void Model::load(QIODevice* io, QPromise<void>& promise) {
    QDataStream stream(io);

    Resource::Model model;
    stream >> model;

    if (promise.isCanceled()) {
        return;
    }

    const auto& mesh_list = model.mesh_list;
    const auto& texture_list = model.texture_list;
    const auto& material_list = model.material_list;

    size_t progress = 0;
    size_t progress_max = mesh_list.count() + texture_list.count() + material_list.count();

    promise.setProgressRange(0, progress_max);
    promise.setProgressValue(0);

    for (const auto& mesh: mesh_list) {
        promise.setProgressValueAndText(++progress, "Loading mesh...");
        auto vertex_buffer_size = sizeof(float) * mesh.vertices.count();
        if (vertex_buffer_size > 0) {
            void* vertex_buffer = nullptr;
            QMetaObject::invokeMethod(this, [this, vertex_buffer_size, &vertex_buffer] {
                auto buf = std::unique_ptr<Buffer>(context_.allocStaticVertexBuffer(vertex_buffer_size));
                if (buf) {
                    buf->bind();
                    vertex_buffer = buf->map(Buffer::WriteOnly);
                    vertex_buffers_.emplace_back(std::move(buf));
                }
            }, Qt::BlockingQueuedConnection);
            if (vertex_buffer == nullptr) {
                return;
            }
            memcpy(vertex_buffer, mesh.vertices.data(), vertex_buffer_size);
        }
        auto index_buffer_size = sizeof(quint32) * mesh.indices.count();
        if (index_buffer_size > 0) {
            void* index_buffer = nullptr;
            QMetaObject::invokeMethod(this, [this, index_buffer_size, &index_buffer] {
                auto buf = std::unique_ptr<Buffer>(context_.allocStaticIndexBuffer(index_buffer_size));
                if (buf) {
                    buf->bind();
                    index_buffer = buf->map(Buffer::WriteOnly);
                    index_buffers_.emplace_back(std::move(buf));
                }
            }, Qt::BlockingQueuedConnection);
            if (index_buffer == nullptr) {
                return;
            }
            memcpy(index_buffer, mesh.indices.data(), index_buffer_size);
        }
        if (promise.isCanceled()) {
            return;
        }
    }

    for (const auto& texture: texture_list) {
        promise.setProgressValueAndText(++progress, "Loading texture...");
        if (promise.isCanceled()) {
            return;
        }
    }

    for (const auto& material: material_list) {
        promise.setProgressValueAndText(++progress, "Loading material...");
        if (promise.isCanceled()) {
            return;
        }
    }
}

VOLCANO_GRAPHICS_END
