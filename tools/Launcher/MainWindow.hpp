//
//
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QWindow>
#include <QOpenGLWindow>

#include <Volcano/World.hpp>
#include <Volcano/Graphics/Context.hpp>
#include <Volcano/Graphics/Target.hpp>

class MainWindow: public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow(Volcano::World &w, QWindow *parent = nullptr);
    ~MainWindow(void) override;

protected:
    void initializeGL(void) override;
    void paintGL(void) override;
    void resizeGL(int w, int h) override;

private:
    Volcano::World &m_world;
    Volcano::Snapshot m_snapshot;
    Volcano::Graphics::Context m_gc;
    Volcano::Graphics::Target m_target;
};

#endif // MAINWINDOW_HPP
