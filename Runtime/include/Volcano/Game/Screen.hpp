//
//
#ifndef VOLCANO_GAME_SCREEN_HPP
#define VOLCANO_GAME_SCREEN_HPP

#include <QSizeF>
#include <QEvent>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QQuickRenderControl>
#include <QQuickItem>

#include <Volcano/Transform.hpp>
#include <Volcano/Graphics/QuickSurface.hpp>
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

class Screen: public Actor {
    Q_OBJECT
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(Transform *transform READ transform)
    Q_PROPERTY(QQmlComponent *component READ component WRITE setComponent NOTIFY componentChanged)

public:
    Screen(QObject *parent = nullptr);
    ~Screen(void) override;

public:
    Transform *transform(void);
    qreal width(void) const;
    void setWidth(qreal v);
    qreal height(void) const;
    void setHeight(qreal v);
    QQmlComponent *component(void);
    void setComponent(QQmlComponent *p);

signals:
    void widthChanged(qreal v);
    void heightChanged(qreal v);
    void componentChanged(QQmlComponent *p);

protected slots:
    void onStatusChanged(QQmlComponent::Status status);

protected:
    void updateSizes(void);
    void onDraw(void) override;
    bool event(QEvent *evt) override;

private:
    Transform m_transform;
    QSizeF m_size;
    QQuickWindow *m_window;
    Graphics::QuickSurface *m_surface;
    QQmlComponent *m_component;
    QQuickItem *m_rootItem;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCREEN_HPP
