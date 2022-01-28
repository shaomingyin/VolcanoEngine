//
//
#include <memory>

#include <Volcano/Game/Screen.hpp>

VOLCANO_GAME_BEGIN

Screen::Screen(QObject *parent):
    Actor(parent),
    m_component(nullptr),
    m_window(nullptr),
    m_surface(nullptr)
{
    m_transform.reset();
}

Screen::~Screen(void)
{
}

Transform *Screen::transform(void)
{
    return &m_transform;
}

qreal Screen::width(void) const
{
    return m_size.width();
}

void Screen::setWidth(qreal v)
{
    if (!qFuzzyCompare(m_size.width(), v)) {
        m_size.setWidth(v);
        updateSizes();
        emit widthChanged(v);
    }
}

qreal Screen::height(void) const
{
    return m_size.height();
}

void Screen::setHeight(qreal v)
{
    if (!qFuzzyCompare(m_size.height(), v)) {
        m_size.setHeight(v);
        updateSizes();
        emit heightChanged(v);
    }
}

QQmlComponent *Screen::component(void)
{
    return m_component;
}

void Screen::setComponent(QQmlComponent *p)
{
    if (m_component == p)
        return;

    m_component = p;
    emit componentChanged(p);

    if (m_window == nullptr) {
        //auto window = std::make_unique<QQuickWindow>(this);
        //if (!window)
        //    return;
        //Q_ASSERT(m_surface == nullptr);
        //auto surface = std::make_unique<Graphics::QuickSurface>(graphicsService()->createQuickSurface(window.get()));
        //if (!surface)
        //    return;
        //m_window = window.release();
        //m_surface = surface.release();
    }

    if (p->isLoading())
        connect(p, &QQmlComponent::statusChanged, this, &Screen::onStatusChanged);
    else
        onStatusChanged(p->status());
}

void Screen::onStatusChanged(QQmlComponent::Status status)
{
    disconnect(m_component, &QQmlComponent::statusChanged, this, &Screen::onStatusChanged);

    if (status != QQmlComponent::Ready)
        return;

    auto rootObject = m_component->create();
    if (m_component->isError()) {
        auto errorList = m_component->errors();
        for (auto &err: errorList)
            qWarning() << err.url() << err.line() << err;
    }

    m_rootItem = qobject_cast<QQuickItem *>(rootObject);
    if (m_rootItem == nullptr) {
        qWarning("Invalid quick component.");
        delete rootObject;
        return;
    }

    m_rootItem->setParentItem(m_window->contentItem());

    updateSizes();
}

void Screen::updateSizes(void)
{
    if (m_rootItem != nullptr)
        m_rootItem->setSize(m_window->size());

    if (m_window != nullptr)
        m_window->setGeometry(0, 0, m_size.width(), m_size.height());
}

void Screen::onDraw(void)
{
    if (m_surface != nullptr) {
        auto gService = graphicsService();
        gService->pushTransform(true);
        gService->setTransform(m_transform.data());
        gService->addQuickSurface(m_surface);
        gService->popTransform();
    }
}

bool Screen::event(QEvent *evt)
{
    if (!isEnabled())
        return false;

    if (m_window == nullptr)
        return Actor::event(evt);

    bool ret;

    switch (evt->type()) {
    case QEvent::Resize:
        m_window->resize(static_cast<QResizeEvent *>(evt)->size());
        updateSizes();
        ret = true;
        break;
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
        {
            auto mouseEvent = static_cast<QMouseEvent *>(evt);
            QMouseEvent mappedEvent(
                        mouseEvent->type(),
                        mouseEvent->position(),
                        mouseEvent->globalPosition(),
                        mouseEvent->button(),
                        mouseEvent->buttons(),
                        mouseEvent->modifiers());
            ret = QCoreApplication::sendEvent(m_window, &mappedEvent);
        }
        break;
    case QEvent::MouseButtonDblClick:
        break;
    default:
        ret = QCoreApplication::sendEvent(m_window, evt);
        break;
    }

    return ret;
}

VOLCANO_GAME_END
