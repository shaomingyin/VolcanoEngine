//
//
#ifndef VOLCANO_LAUNCHER_MAINWINDOW_HPP
#define VOLCANO_LAUNCHER_MAINWINDOW_HPP

#include <QUrl>
#include <QTimerEvent>
#include <QElapsedTimer>
#include <QScreen>
#include <QWindow>
#include <QOpenGLContext>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickRenderControl>

#include <Volcano.hpp>

class MainWindow: public QWindow
{
    Q_OBJECT

public:
    MainWindow(QScreen *screen = nullptr);
    ~MainWindow(void) override;

public:
    bool init(void);
    bool load(const QUrl &url);
    Volcano::World *world(void);

protected:
    void timerEvent(QTimerEvent *evt) override;

private slots:
    void startup(void);

private:
    void frame(float elapsed);

private:
    QOpenGLContext *m_glContext;
    QQmlEngine m_qmlEngine;
    QQmlComponent *m_qmlComponent;
    Volcano::OpenGL::Renderer *m_renderer;
    Volcano::OpenGL::View m_view;
    Volcano::OpenGL::Target m_target;
    Volcano::World *m_world;
    int m_frameTimer;
    QElapsedTimer m_elapsedTimer;
};

VOLCANO_INLINE Volcano::World *MainWindow::world(void)
{
    return m_world;
}

#endif // VOLCANO_LAUNCHER_MAINWINDOW_HPP
