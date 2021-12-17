//
//
#ifndef VOLCANO_GRAPHICS_VISUAL_HPP
#define VOLCANO_GRAPHICS_VISUAL_HPP

#include <QObject>

#include <Volcano/Box.hpp>
#include <Volcano/Sphere.hpp>
#include <Volcano/Cylinder.hpp>
#include <Volcano/Plane.hpp>
#include <Volcano/Game/Visual.hpp>
#include <Volcano/Game/Material.hpp>
#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

class Visual: public QObject {
    Q_OBJECT

public:
    Visual(Game::Visual *p, QObject *parent = nullptr);
    ~Visual(void) override;

public:
    Game::Visual *gameVisual(void);
    bool isValid(void) const;
    void buildView(View *view, const Camera &cam) const;

private slots:
    void onShapeChanged(Shape *shape);

private:
    Game::Visual *m_gameVisual;
    Box *m_box;
    Cylinder *m_cylinder;
    Sphere *m_sphere;
    Plane *m_plane;
    Game::Mesh *m_gameMesh;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VISUAL_HPP
