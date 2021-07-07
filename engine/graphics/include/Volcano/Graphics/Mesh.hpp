//
//
#ifndef VOLCANO_GRAPHICS_MESH_HPP
#define VOLCANO_GRAPHICS_MESH_HPP

#include <QObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Renderable.hpp>

VOLCANO_GRAPHICS_BEGIN

class Mesh: public Renderable {
    Q_OBJECT

public:
    Mesh(QObject *parent = nullptr);
    ~Mesh(void) override;

public:

private:
    QIODevice *m_vertexBuffer;
    QIODevice *m_vertexIndexBuffer;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RESOURCE_HPP
