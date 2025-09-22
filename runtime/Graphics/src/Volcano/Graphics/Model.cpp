//
//
#include <QScopeGuard>
#include <QDataStream>
#include <QOpenGLContext>

#include <Volcano/Graphics/Model.h>

VOLCANO_GRAPHICS_BEGIN

Model::Model(Context& context, QObject* parent)
    : Drawable(context, parent)
    , vertex_current_(nullptr) {
}

void Model::enable() {
    if (vertex_current_ == nullptr) {
        Q_ASSERT(vertex_current_ == nullptr);
    }
    Drawable::enable();
}

void Model::disable() {
    vertex_current_ = nullptr;
    Drawable::disable();
}

void Model::setDetailLevel(float level) {

}

void Model::tick(Duration elapsed) {
    Drawable::tick(elapsed);
}

void Model::draw() {
    if (Q_LIKELY(vertex_current_ != nullptr)) {
    }

    Drawable::draw();
}
#if 0
void Model::onSourceChanged(const QUrl& v) {
    if (!isEnabled()) {
        return;
    }

    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);
    Q_ASSERT(QOpenGLContext::currentContext()->thread()->isCurrentThread());

    if (loading_task_.isRunning()) {
        loading_task_.cancel();
        loading_task_.waitForFinished();
    }

    vertex_current_ = nullptr;
    for (int i = 0; i < static_cast<int>(Resource::DetailLevel::Max); ++i) {
        vertex_details_[i].reset();
    }

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
        QtConcurrent::run([this, content] {
            auto context_guard = qScopeGuard([content] {
                delete content;
            });
            QDataStream stream(content);
            Resource::Model model;
            stream >> model;
            return model;
        }).then(this, [this, promise = std::move(promise)] (Resource::Model model) mutable {
            load(model, promise);
        });
    };

    if (content->isFinished()) {
        on_finished();
    } else {
        connect(content, &QNetworkReply::finished, this, std::move(on_finished));
    }
}

void Model::load(const Resource::Model& model, QPromise<void>& promise) {
    if (promise.isCanceled()) {
        return;
    }

    int n = model.count();
    for (auto it = model.begin(); it != model.end(); ++it) {
        it.key()
        const auto& mesh_list = lm.mesh_list;
        const auto& texture_list = model.texture_list;
        const auto& material_list = model.material_list;

        size_t progress_max = mesh_list.count() + texture_list.count() + material_list.count();

        promise.setProgressRange(0, progress_max);
        promise.setProgressValue(0);

        for (const auto& mesh: mesh_list) {
            promise.setProgressValueAndText(++progress, "Loading mesh...");
            loadMesh(mesh, promise);
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
}

void Model::loadMesh(const Resource::Mesh& mesh, QPromise<void>& promise) {
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
}
#endif

VOLCANO_GRAPHICS_END
