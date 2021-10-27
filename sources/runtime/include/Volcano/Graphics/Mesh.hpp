//
//
#ifndef VOLCANO_GRAPHICS_MESH_HPP
#define VOLCANO_GRAPHICS_MESH_HPP

#include <QFuture>

#include <Volcano/Game/Resource.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Mesh: public Game::Resource {
    Q_OBJECT

public:
    Mesh(QObject *parent = nullptr);
    virtual ~Mesh(void);

private:
    void onSourceChanged(const QUrl &v) override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MESH_HPP
