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

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Buffer.h>
#include <Volcano/Graphics/Drawable.h>

VOLCANO_GRAPHICS_BEGIN

class Model: public Drawable {
    Q_OBJECT

public:
    Model(Context& context, QObject* parent = nullptr);

public:
    void enable() override;
    void disable() override;
    void setDetailLevel(float level) override;

protected:
    void tick(Duration elapsed) override;
    void draw() override;

private:
#if 0
    void load(const Resource::Model& model, QPromise<void>& promise);
    void loadMesh(const Resource::Mesh& mesh, QPromise<void>& promise);
#endif

private:
    struct VertexData {
        QOpenGLVertexArrayObject vao_;
        std::vector<std::unique_ptr<Buffer>> vertex_buffers_;
        std::vector<std::unique_ptr<Buffer>> index_buffers_;
    };

    //using VertexDetails = std::array<std::unique_ptr<VertexData>, static_cast<int>(Resource::DetailLevel::Max)>;

    QFuture<void> loading_task_;
    //VertexDetails vertex_details_;
    VertexData* vertex_current_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MODEL_H
