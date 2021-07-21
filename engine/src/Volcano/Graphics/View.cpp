//
//
#include <QThread>

#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

View::Transform::Transform(void):
    translate(0.0f, 0.0f, 0.0f),
    scale(1.0f, 1.0f, 1.0f),
    rotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 0.0f, 1.0f), 0.0f))
{
}

void View::Transform::reset(void)
{
    translate = QVector3D(0.0f, 0.0f, 0.0f);
    scale = QVector3D(1.0f, 1.0f, 1.0f);
    rotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 0.0f, 1.0f), 0.0f);
}

View::View(QObject *parent):
    QObject(parent),
    m_gl(nullptr),
    m_rendering(0)
{
    m_def[0].transformStack.emplaceBack();
    m_def[1].transformStack.emplaceBack();
}

View::~View(void)
{
}

const QSize View::size(void) const
{
    return m_size;
}

void View::setSize(const QSize &v)
{
    m_size = v;

    // TODO
}

bool View::begin(void)
{
    if (m_state.testAndSetRelaxed(0, 1)) {
        auto &d = m_def[!m_rendering];

        d.isClear = false;

        auto &t = d.transformStack.back();
        t.reset();

        return true;
    }

    return false;
}

void View::end(void)
{
    Q_ASSERT(m_state == 1);

    m_rendering = !m_rendering;
    m_state = 2;
}

void View::beforeRender(void)
{
    m_state.testAndSetRelaxed(2, 3);
}

void View::render(void)
{
    if (m_state == 3) {
        renderDef(m_def[m_rendering]);
        m_state = 0;
    }
}

void View::setClear(bool v)
{
    auto &d = m_def[!m_rendering];
    d.isClear = v;
}

void View::setClearColor(const QColor &v)
{
    auto &d = m_def[!m_rendering];
    d.clearColor = v;
}

void View::lookAt(const QVector3D &position,  const QVector3D &direction, const QVector3D &up)
{
    Q_ASSERT(direction == direction.normalized());
    Q_ASSERT(up == up.normalized());

    auto &d = m_def[!m_rendering];
    d.position = position;
    d.direction = direction;
    d.up = up;
}

void View::setProjection(const QMatrix4x4 &v)
{
    auto &d = m_def[!m_rendering];
    d.projection = v;
}

void View::pushTransform(void)
{
    auto &d = m_def[!m_rendering];
    auto &r = d.transformStack.emplaceBack();
}

void View::popTransform(void)
{
    auto &d = m_def[!m_rendering];
    Q_ASSERT(d.transformStack.size() > 1);
    d.transformStack.pop_back();
}

void View::resetTransform(void)
{
    auto &d = m_def[!m_rendering];
    auto &t = d.transformStack.back();
    t.reset();
}

void View::translate(const QVector3D &v)
{
    auto &d = m_def[!m_rendering];
    auto &t = d.transformStack.back();
    t.translate = v;
}

void View::translateTo(const QVector3D &v)
{
    auto &d = m_def[!m_rendering];
    auto &t = d.transformStack.back();
    t.translate += v;
}

void View::scale(const QVector3D &v)
{
    auto &d = m_def[!m_rendering];
    auto &t = d.transformStack.back();
    t.scale = v;
}

void View::scaleTo(const QVector3D &v)
{
    auto &d = m_def[!m_rendering];
    auto &t = d.transformStack.back();
    t.scale *= v;
}

void View::rotate(float angle, const QVector3D &axis)
{
    auto &d = m_def[!m_rendering];
    auto &t = d.transformStack.back();
    t.rotation = QQuaternion::fromAxisAndAngle(axis, angle);
}

void View::rotateTo(float angle, const QVector3D &axis)
{
    auto &d = m_def[!m_rendering];
    auto &t = d.transformStack.back();
    t.rotation *= QQuaternion::fromAxisAndAngle(axis, angle);
}

void View::renderDef(Def &d)
{
    auto gl = glFunctions();
    Q_ASSERT(gl != nullptr);

    if (d.isClear) {
        gl->glClearColor(d.clearColor.redF(), d.clearColor.greenF(), d.clearColor.blueF(), d.clearColor.alphaF());
        gl->glClear(GL_COLOR_BUFFER_BIT);
    }
}

VOLCANO_GRAPHICS_END
