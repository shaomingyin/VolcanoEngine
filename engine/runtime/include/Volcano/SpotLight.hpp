//
//
#ifndef VOLCANO_SPOTLIGHT_HPP
#define VOLCANO_SPOTLIGHT_HPP

#include <QVector3D>

#include <Volcano/Common.hpp>
#include <Volcano/DirectionalLight.hpp>

VOLCANO_BEGIN

class SpotLight: public DirectionalLight {
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)

public:
    SpotLight(QObject *parent = nullptr);
    ~SpotLight(void) override;

public:
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    const QVector3D &scale(void) const;
    void setScale(const QVector3D &v);

signals:
    void positionChanged(const QVector3D &v);
    void scaleChanged(const QVector3D &v);

private:
    QVector3D m_position;
    QVector3D m_scale;
};

VOLCANO_END

#endif // VOLCANO_SPOTLIGHT_HPP
