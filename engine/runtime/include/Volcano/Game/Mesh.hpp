//
//
#ifndef VOLCANO_GAME_MESH_HPP
#define VOLCANO_GAME_MESH_HPP

#include <QUrl>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class Mesh: public Component {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

public:
    Mesh(QObject *parent = nullptr);
    ~Mesh(void) override;

public:
    const QUrl &source(void) const;
    void setSource(const QUrl &url);

signals:
    void sourceChanged(const QUrl &url);

private:
    QUrl m_source;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MESH_HPP
