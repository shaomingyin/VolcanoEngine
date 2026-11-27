//
//
#ifndef VOLCANO_LAUNCHER_MAINWINDOW_H
#define VOLCANO_LAUNCHER_MAINWINDOW_H

#include <memory>

#include <QUrl>
#include <QTimerEvent>
#include <QOpenGLWindow>

#include <Volcano/World/Common.h>
#include <Volcano/World/Scene.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Launcher/Common.h>

VOLCANO_LAUNCHER_BEGIN

class MainWindow: public QOpenGLWindow {
    Q_OBJECT

public:
    MainWindow();

protected:
    void initializeGL() override;
    void paintGL() override;
    void paintOverGL() override;
    void paintUnderGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent *p) override;
    void keyReleaseEvent(QKeyEvent *p) override;
    void mouseMoveEvent(QMouseEvent *p) override;
    void mousePressEvent(QMouseEvent *p) override;
    void mouseReleaseEvent(QMouseEvent *p) override;
    void timerEvent(QTimerEvent* p) override;

private:
    void frame(World::Clock::duration elapsed);
    void idleFrame(World::Clock::duration elapsed);
    void loadingFrame(World::Clock::duration elapsed);
    void readyFrame(World::Clock::duration elapsed);
    void playingFrame(World::Clock::duration elapsed);
    void pausedFrame(World::Clock::duration elapsed);
    void stoppingFrame(World::Clock::duration elapsed);
    void errorFrame(World::Clock::duration elapsed);

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

    World::Clock::time_point frame_last_;
    int frame_count_;
    int frame_count_per_second_;
    int frame_timer_;
    int frame_count_timer_;

    World::Scene scene_;
    std::unique_ptr<Graphics::Renderer> graphics_renderer_;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_MAINWINDOW_H
