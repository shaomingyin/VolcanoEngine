//
//
#ifndef VOLCANO_GRAPHICS_SERVICE_HPP
#define VOLCANO_GRAPHICS_SERVICE_HPP

#include <QColor>
#include <QWindow>
#include <QQuickWindow>

#include <Volcano/Service.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Mesh.hpp>
#include <Volcano/Graphics/Material.hpp>
#include <Volcano/Graphics/QuickSurface.hpp>

VOLCANO_GRAPHICS_BEGIN

class Service: public Volcano::Service {
    Q_OBJECT

public:
    Service(QObject *parent = nullptr);
    ~Service(void) override;

public:
    virtual QWindow *window(void) = 0;
    virtual Mesh *createMesh() = 0;
    virtual void addMesh(Mesh *p) = 0;
    virtual Material *createMaterial() = 0;
    virtual void bindMaterial(Material *p) = 0;
    virtual QuickSurface *createQuickSurface(QQuickWindow *window) = 0;
    virtual void addQuickSurface(QuickSurface *surface) = 0;
    virtual void beginFrame(void) = 0;
    virtual void endFrame(void) = 0;
    void enableClear(void);
    void disableClear(void);
    void setClearColor(const QColor &v);
    void lookAt(const QVector3D &position, const QVector3D &direction, const QVector3D &up);
    void perspective(float fov, float ratio, float nearPlane, float farPlane);
    void setAmbientLight(const QColor &v);

protected:
    bool m_isClearEnabled;
    QColor m_clearColor;
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
    float m_fov;
    float m_ratio;
    float m_nearPlane;
    float m_farPlane;
    QColor m_ambientLight;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_SERVICE_HPP
