//
//
#include <QThread>

#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

View::View(QObject *parent):
    QObject(parent),
    m_gl(nullptr)
{
}

View::~View(void)
{
}

bool View::init(const QSize &v)
{
    Q_ASSERT(QThread::currentThread() == thread());

    Q_ASSERT(m_gl == nullptr);
    m_gl = glFunctions();
    if (m_gl == nullptr)
        return false;

    setSize(v);

    return true;
}

const QSize View::size(void) const
{
    return m_size;
}

void View::setSize(const QSize &v)
{
    Q_ASSERT(QThread::currentThread() == thread());

    m_size = v;

    // TODO
}

void View::reset(void)
{
}

void View::render(void)
{
    Q_ASSERT(QThread::currentThread() == thread());
    Q_ASSERT(m_gl != nullptr);

    // TODO
}

VOLCANO_GRAPHICS_END
