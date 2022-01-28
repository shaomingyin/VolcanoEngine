//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_SERVICE_HPP
#define VOLCANO_GRAPHICS_OPENGL_SERVICE_HPP

#include <QSize>
#include <QColor>
#include <QVector3D>

#include <Volcano/Graphics/Service.hpp>
#include <Volcano/Graphics/OpenGL/Window.hpp>
#include <Volcano/Graphics/OpenGL/Common.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class Service: public Volcano::Graphics::Service {
public:
    Service(QObject *parent = nullptr);
    ~Service(void) override;

public:
    bool init(int width, int height);
    void resize(int width, int height);
    QWindow *window(void) override;
    Mesh *createMesh() override;
    void addMesh(Mesh *p) override;
    Material *createMaterial() override;
    void bindMaterial(Material *p) override;
    QuickSurface *createQuickSurface(QQuickWindow *window) override;
    void addQuickSurface(QuickSurface *surface) override;
    void beginFrame(void) override;
    void endFrame(void) override;

protected:
    bool onStart(void) override;
    void onStop(void) override;

private:
    Window m_window;
};

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_SERVICE_HPP
