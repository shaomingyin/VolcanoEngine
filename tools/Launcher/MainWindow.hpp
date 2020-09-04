//
//
#ifndef VOLCANO_LAUNCHER_MAINWINDOW_HPP
#define VOLCANO_LAUNCHER_MAINWINDOW_HPP

#include <QUrl>
#include <QTimerEvent>
#include <QElapsedTimer>
#include <QScreen>
#include <QOpenGLWindow>
#include <QOpenGLContext>

#include <Volcano.hpp>

class MainWindow: public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow(void);
    ~MainWindow(void) override;

public:

protected:
    void initializeGL(void) override;
    void paintGL(void) override;
    void paintOverGL(void) override;
    void paintUnderGL(void) override;
    void resizeGL(int w, int h) override;

private:
    Volcano::VM m_vm;
    Volcano::Drivers::Graphics::Renderer *m_renderer;
    Volcano::Drivers::Graphics::Target m_target;
};

#endif // VOLCANO_LAUNCHER_MAINWINDOW_HPP
