//
//
#ifndef VOLCANO_MESH_HPP
#define VOLCANO_MESH_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Resource.hpp>
#include <Volcano/OpenGL/Buffer.hpp>

VOLCANO_BEGIN

class VOLCANO_API Mesh: public Resource
{
    Q_OBJECT

public:
    Mesh(Node *parent = nullptr);
    ~Mesh(void) override;

protected:
    void onSourceChanged(const QUrl &url) override;

private:
    void createBuffers(void);
    void destroyBuffers(void);

private:
    OpenGL::Buffer *m_vertexBuffer;
    OpenGL::Buffer *m_vertexIndexBuffer;
};

VOLCANO_END

#endif // VOLCANO_MESH_HPP
