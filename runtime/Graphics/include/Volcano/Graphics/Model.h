//
//
#ifndef VOLCANO_GRAPHICS_MODEL_H
#define VOLCANO_GRAPHICS_MODEL_H

#include <array>
#include <memory>
#include <vector>

#include <QMap>
#include <QFuture>
#include <QObject>
#include <QIODevice>
#include <QOpenGLVertexArrayObject>

#include <Volcano/World/Entity.h>
#include <Volcano/World/Model.h>
#include <Volcano/Resource/Model.h>
#include <Volcano/Resource/Common.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Buffer.h>
#include <Volcano/Graphics/Drawable.h>

VOLCANO_GRAPHICS_BEGIN

class Model: public Drawable {
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

    void enable() override;
    void disable() override;
    void setDetailLevel(float level) override;

protected:
    void tick(Duration elapsed) override;
    void draw() override;

private slots:
    void onSourceChanged(const QUrl& v);

private:
    void load(const Resource::Model& model, QPromise<void>& promise);
    void loadMesh(const Resource::Mesh& mesh, QPromise<void>& promise);

private:
    struct VertexData {
        QOpenGLVertexArrayObject vao_;
        std::vector<std::unique_ptr<Buffer>> vertex_buffers_;
        std::vector<std::unique_ptr<Buffer>> index_buffers_;
    };

    using VertexDetails = std::array<std::unique_ptr<VertexData>, static_cast<int>(Resource::DetailLevel::Max)>;

    World::Entity* world_entity_;
    World::Model* world_model_;
    QFuture<void> loading_task_;
    VertexDetails vertex_details_;
    VertexData* vertex_current_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MODEL_H
