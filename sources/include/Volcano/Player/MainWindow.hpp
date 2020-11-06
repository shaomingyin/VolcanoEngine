//
//
#ifndef VOLCANO_PLAYER_MAINWINDOW_HPP
#define VOLCANO_PLAYER_MAINWINDOW_HPP

#include <QUrl>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QOpenGLWindow>
#include <QQmlApplicationEngine>

#include <Volcano/SpinLock.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/Player/Common.hpp>

VOLCANO_PLAYER_BEGIN

class MainWindow: public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow(QWindow *parent = nullptr);
    ~MainWindow(void) override;

public:
    bool init(const QUrl &gameUrl);
    QUrl gameUrl(void) const;

protected:
    void timerEvent(QTimerEvent *event);
    void initializeGL(void) override;
    void paintGL(void) override;
    void resizeGL(int w, int h) override;

private:
    void gameThread(void);
    void frame(float elapsed);

private slots:
    void objectCreated(QObject *object, const QUrl &url);
    void quit(void);
    void warnings(const QList<QQmlError> &warnings);

private:
    QUrl m_gameUrl;
    bool m_gameRunning;
    QThread *m_gameThread;
    Game::World *m_gameWorld;
    QMutex m_gameMutex;
    QWaitCondition m_gameCond;
    int m_frameTimer;
    int m_frameCountTimer;
    int m_frameCount;
    int m_frameCountPerSecond;
    Graphics::Renderer *m_renderer;
    Graphics::View m_viewFlip[2];
    int m_viewRendering;
};

VOLCANO_INLINE QUrl MainWindow::gameUrl(void) const
{
    return m_gameUrl;
}

VOLCANO_PLAYER_END

#endif // VOLCANO_PLAYER_MAINWINDOW_HPP
