//
//
#ifndef VOLCANO_LAUNCHER_MAINWINDOW_H
#define VOLCANO_LAUNCHER_MAINWINDOW_H

#include <memory>

#include <QUrl>
#include <QTimerEvent>
#include <QQuickView>

#include <Volcano/World/Scene.h>
#include <Volcano/Acoustics/Space.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Launcher/Common.h>

VOLCANO_LAUNCHER_BEGIN

class MainWindow: public QQuickView {
    Q_OBJECT

public:
    MainWindow();

protected:
    void keyPressEvent(QKeyEvent *p) override;
    void keyReleaseEvent(QKeyEvent *p) override;
    void mouseMoveEvent(QMouseEvent *p) override;
    void mousePressEvent(QMouseEvent *p) override;
    void mouseReleaseEvent(QMouseEvent *p) override;
    void timerEvent(QTimerEvent* p) override;

private:
    void frame(Duration elapsed);
    void idleFrame(Duration elapsed);
    void loadingFrame(Duration elapsed);
    void readyFrame(Duration elapsed);
    void playingFrame(Duration elapsed);
    void pausedFrame(Duration elapsed);
    void stoppingFrame(Duration elapsed);
    void errorFrame(Duration elapsed);

private:
    enum class State {
        Idle = 0,
        Loading,
        Ready,
        Playing,
        Paused,
        Stopping,
        Error
    };

private:
    State state_;

    TimePoint frame_last_;
    int frame_count_;
    int frame_count_per_second_;
    int frame_timer_;
    int frame_count_timer_;

    World::Scene* scene_;
    std::unique_ptr<Acoustics::Space> acoustics_space_;
    std::unique_ptr<Graphics::Renderer> graphics_renderer_;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_MAINWINDOW_H
