//
//
#ifndef VOLCANO_GAME_MESH_HPP
#define VOLCANO_GAME_MESH_HPP

#include <QUrl>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Material.hpp>

VOLCANO_GAME_BEGIN

class Mesh: public Component {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(Material *material READ material WRITE setMaterial NOTIFY materialChanged)

public:
    Mesh(QObject *parent = nullptr);
    Mesh(Context *context, QObject *parent = nullptr);
    ~Mesh(void) override;

public:
    const QUrl &source(void) const;
    void setSource(const QUrl &v);
    Material *material(void);
    void setMaterial(Material *p);

signals:
    void sourceChanged(const QUrl &v);
    void materialChanged(Material *p);

protected:
    void draw(void) override;

private:
    void reload(void);

private:
    QUrl m_source;
    Graphics::Mesh *m_graphicsMesh;
    Material *m_material;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MESH_HPP
