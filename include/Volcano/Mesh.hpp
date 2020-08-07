//
//
#ifndef VOLCANO_MESH_HPP
#define VOLCANO_MESH_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Resource.hpp>
#include <Volcano/Graphics/Memory.hpp>
#include <Volcano/Graphics/Buffer.hpp>

VOLCANO_BEGIN

class VOLCANO_API Mesh: public Resource
{
    Q_OBJECT

public:
    Mesh(Node *parent = nullptr);
    ~Mesh(void) override;

public:
    bool init(Graphics::Memory &gm);

protected slots:
    void onSourceChanged(const QUrl &url);

protected:
    void release(void);

private:
    Graphics::Buffer *m_vertexBuffer;
    Graphics::Buffer *m_vertexIndexBuffer;
};

VOLCANO_END

#endif // VOLCANO_MESH_HPP
