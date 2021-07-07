//
//
#include <Volcano/Graphics/VisibleSet.hpp>

VOLCANO_GRAPHICS_BEGIN

VisibleSet::VisibleSet(void):
    m_lastAddedRenderable(nullptr)
{
    pushTransform();
}

VisibleSet::~VisibleSet(void)
{
}

void VisibleSet::reset(void)
{
    m_transformStack.clear();
    pushTransform();

    m_transformedRenderableVector.clear();
}

void VisibleSet::pushTransform(void)
{
    m_transformStack.emplaceBack();
    loadIdienty();
}

void VisibleSet::popTransform(void)
{
    Q_ASSERT(m_transformStack.size() > 1);

    m_transformStack.pop_back();
    m_lastAddedRenderable = nullptr;
}

void VisibleSet::loadIdienty(void)
{
    auto &ref = m_transformStack[m_transformStack.size() - 1];
    ref.translate = QVector3D(0.0f, 0.0f, 0.0f);
    ref.scale = QVector3D(1.0f, 1.0f, 1.0f);
    ref.rotation = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 0.0f);
    m_lastAddedRenderable = nullptr;
}

void VisibleSet::translate(float x, float y, float z)
{
    auto &ref = m_transformStack[m_transformStack.size() - 1];
    ref.translate += QVector3D(x, y, z);
    m_lastAddedRenderable = nullptr;
}

void VisibleSet::scale(float x, float y, float z)
{
    auto &ref = m_transformStack[m_transformStack.size() - 1];
    ref.scale *= QVector3D(x, y, z);
    m_lastAddedRenderable = nullptr;
}

void VisibleSet::rotate(float angle, float x, float y, float z)
{
    auto &ref = m_transformStack[m_transformStack.size() - 1];
    ref.rotation *= QQuaternion::fromAxisAndAngle(x, y, z, angle);
    m_lastAddedRenderable = nullptr;
}

void VisibleSet::add(Renderable *renderable)
{
    if (renderable != m_lastAddedRenderable) {
        auto &ref = m_transformedRenderableVector.emplaceBack();
        ref.first = m_transformStack[m_transformStack.size() - 1];
        ref.second = renderable;
        m_lastAddedRenderable = renderable;
    }
}

const VisibleSet::TransformedRenderableVector &VisibleSet::renderables(void) const
{
    return m_transformedRenderableVector;
}

VOLCANO_GRAPHICS_END
