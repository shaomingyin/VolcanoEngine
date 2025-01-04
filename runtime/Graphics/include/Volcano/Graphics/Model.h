//
//
#ifndef VOLCANO_GRAPHICS_MODEL_H
#define VOLCANO_GRAPHICS_MODEL_H

#include <memory>
#include <vector>

#include <QFuture>
#include <QObject>
#include <QIODevice>
#include <QOpenGLVertexArrayObject>

#include <Volcano/Resource/Model.h>
#include <Volcano/World/Entity.h>
#include <Volcano/World/Model.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Buffer.h>
#include <Volcano/Graphics/Context.h>

VOLCANO_GRAPHICS_BEGIN

class Model: public QObject {
    Q_OBJECT

public:
    Model(Context& context, World::Entity* world_entity, World::Model* world_model, QObject* parent = nullptr);

public:
    World::Entity* worldEntity() {
        return world_entity_;
    }

    const World::Entity* worldEntity() const {
        return world_entity_;
    }

    World::Model* worldModel() {
        return world_model_;
    }

    const World::Model* worldModel() const {
        return world_model_;
    }

private slots:
    void onSourceChanged(const QUrl& v);

private:
    void load(QIODevice* io, QPromise<void>& promise);

private:
    Context& context_;
    World::Entity* world_entity_;
    World::Model* world_model_;
    QFuture<void> loading_task_;
    QOpenGLVertexArrayObject vao_;
    std::vector<std::unique_ptr<Buffer>> vertex_buffers_;
    std::vector<std::unique_ptr<Buffer>> index_buffers_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MODEL_H
