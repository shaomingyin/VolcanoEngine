//
//
#ifndef VOLCANO_GAME_MESH_HPP
#define VOLCANO_GAME_MESH_HPP

#include <QFuture>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Resource.hpp>

VOLCANO_GAME_BEGIN

class Mesh: public Resource {
    Q_OBJECT

public:
    Mesh(QObject *parent = nullptr);
    virtual ~Mesh(void);

private:
    void onSourceChanged(const QUrl &v) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MESH_HPP
