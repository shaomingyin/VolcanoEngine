//
//
#include <Volcano/Graphics/Visual.hpp>

VOLCANO_GRAPHICS_BEGIN

Visual::Visual(Game::Visual *p, QObject *parent):
    QObject(parent),
    m_gameVisual(p)
{
    Q_ASSERT(p != nullptr);

    onShapeChanged(p->shape());
    connect(p, &Game::Visual::shapeChanged, this, &Visual::onShapeChanged);
}

Visual::~Visual(void)
{
}

Game::Visual *Visual::gameVisual(void)
{
    return m_gameVisual;
}

bool Visual::isValid(void) const
{
    return (m_box != nullptr ||
            m_cylinder != nullptr ||
            m_sphere != nullptr ||
            m_plane != nullptr ||
            m_gameMesh != nullptr);
}

void Visual::buildView(View *view, const Camera &cam) const
{
    Q_ASSERT(view != nullptr);

    view->translateTo(m_gameVisual->offset());
    view->scaleTo(m_gameVisual->scale());
    view->rotateTo(m_gameVisual->rotation());

    if (m_gameMesh != nullptr) {
        // TODO
        return;
    }

    if (m_box != nullptr) {
        // TODO
        return;
    }

    if (m_cylinder != nullptr) {
        // TODO
        return;
    }

    if (m_sphere != nullptr) {
        // TODO
        return;
    }

    if (m_plane != nullptr) {
        // TODO
        return;
    }
}

void Visual::onShapeChanged(Shape *shape)
{
    m_box = nullptr;
    m_cylinder = nullptr;
    m_sphere = nullptr;
    m_plane = nullptr;
    m_gameMesh = nullptr;

    m_box = qobject_cast<Box *>(shape);
    if (m_box != nullptr)
        return;

    m_cylinder = qobject_cast<Cylinder *>(shape);
    if (m_cylinder != nullptr)
        return;

    m_sphere = qobject_cast<Sphere *>(shape);
    if (m_sphere != nullptr)
        return;

    m_plane = qobject_cast<Plane *>(shape);
    if (m_plane != nullptr)
        return;

    m_gameMesh = qobject_cast<Game::Mesh *>(shape);
}

VOLCANO_GRAPHICS_END
