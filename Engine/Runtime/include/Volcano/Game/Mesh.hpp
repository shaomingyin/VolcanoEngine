//
//
#ifndef VOLCANO_GAME_MESH_HPP
#define VOLCANO_GAME_MESH_HPP

#include <QUrl>

#include <Volcano/Shape.hpp>
#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Mesh: public Shape {
    Q_OBJECT

public:
    Mesh(QObject *parent = nullptr);
    ~Mesh(void) override;

public:
    QUrl source(void) const;
    void setSource(QUrl v);

signals:
    void sourceChanged(const QUrl &v);

private:
    QUrl m_source;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MESH_HPP
