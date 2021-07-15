//
//
#ifndef VOLCANO_EDITOR_CAMERAVIEW_HPP
#define VOLCANO_EDITOR_CAMERAVIEW_HPP

#include <QQuickWidget>

#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class CameraView: public QQuickWidget {
    Q_OBJECT

public:
    CameraView(QWidget *parent = nullptr);
    ~CameraView(void) override;

public:
    bool init(void);

private:
    Graphics::Camera m_graphicsCamera;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_CAMERAVIEW_HPP
