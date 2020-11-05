//
//
#ifndef VOLCANO_PLAYER_MAINWINDOW_HPP
#define VOLCANO_PLAYER_MAINWINDOW_HPP

#include <QUrl>
#include <QThread>
#include <QAtomicInt>
#include <QOpenGLWindow>

#include <Volcano/Game/VM.hpp>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/Player/Common.hpp>

VOLCANO_PLAYER_BEGIN

class MainWindow: public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow(const QUrl &gameUrl, QWindow *parent = nullptr);
    ~MainWindow(void) override;

public:
    QUrl gameUrl(void) const;

protected:
    void initializeGL(void) override;
    void paintGL(void) override;
    void resizeGL(int w, int h) override;

private:
    void gameThread(void);

private:
    QUrl m_gameUrl;
    QThread *m_gameThread;
    Graphics::View m_viewFlip[2];
    QAtomicInt m_viewRendering;
    Graphics::Renderer m_renderer;
};

VOLCANO_INLINE QUrl MainWindow::gameUrl(void) const
{
    return m_gameUrl;
}

VOLCANO_PLAYER_END

#endif // VOLCANO_PLAYER_MAINWINDOW_HPP
