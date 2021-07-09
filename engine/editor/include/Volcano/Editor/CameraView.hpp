//
//
#ifndef VOLCANO_EDITOR_CAMERAVIEW_HPP
#define VOLCANO_EDITOR_CAMERAVIEW_HPP

#include <QOpenGLWidget>

#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class CameraView: public QOpenGLWidget {
    Q_OBJECT

public:
    CameraView(QWidget *parent = nullptr);
    ~CameraView(void) override;

private:
    void initializeGL(void) override;
    void paintGL(void) override;
    void resizeGL(int w, int h) override;

private:
    Graphics::Camera m_graphicsCamera;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_CAMERAVIEW_HPP
