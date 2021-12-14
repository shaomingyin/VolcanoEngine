//
//
#ifndef VOLCANO_GRAPHICS_MESH_HPP
#define VOLCANO_GRAPHICS_MESH_HPP

#include <QList>
#include <QObject>

#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Buffer.hpp>

VOLCANO_GRAPHICS_BEGIN

class Mesh: public QObject {
    Q_OBJECT

public:
    Mesh(Game::Mesh *p, QObject *parent = nullptr);
    ~Mesh(void) override;

public:
    Game::Mesh *gameMesh(void);

private slots:
    void onSourceChanged(const QUrl &v);

private:
    using BufferList = QList<Buffer *>;

    Game::Mesh *m_gameMesh;
    BufferList m_vertexBufferList;
    BufferList m_vertexIndexBufferList;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MESH_HPP
