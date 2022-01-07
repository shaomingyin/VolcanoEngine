//
//
#ifndef VOLCANO_GRAPHICS_VISUAL_HPP
#define VOLCANO_GRAPHICS_VISUAL_HPP

#include <QObject>
#include <QVector3D>
#include <QQuaternion>
#include <QQmlListProperty>

#include <Volcano/Rotation.hpp>
#include <Volcano/Shape.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Material.hpp>

VOLCANO_GRAPHICS_BEGIN

class Visual: public Game::Component {
    Q_OBJECT
    Q_PROPERTY(QVector3D offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(Rotation *rotation READ rotation)
    Q_PROPERTY(Shape *shape READ shape WRITE setShape NOTIFY shapeChanged)
    Q_PROPERTY(Material *material READ material WRITE setMaterial NOTIFY materialChanged)

public:
    Visual(QObject *parent = nullptr);
    ~Visual(void) override;

public:
    const QVector3D &offset(void) const;
    void setOffset(const QVector3D &v);
    const QVector3D &scale(void) const;
    void setScale(const QVector3D &v);
    Rotation *rotation(void);
    Shape *shape(void);
    void setShape(Shape *p);
    Material *material(void);
    void setMaterial(Material *p);

signals:
    void offsetChanged(const QVector3D &v);
    void scaleChanged(const QVector3D &v);
    void shapeChanged(Shape *p);
    void materialChanged(Material *p);

private:
    QVector3D m_offset;
    QVector3D m_scale;
    Rotation m_rotation;
    Shape *m_shape;
    Material *m_material;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VISUAL_HPP
