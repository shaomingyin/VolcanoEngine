//
//
#include <QScopedPointer>
#include <QElapsedTimer>
#include <QCoreApplication>

#include <Volcano/Game/ContextImpl.hpp>
#include <Volcano/Launcher/GameThread.hpp>

VOLCANO_LAUNCHER_BEGIN

GameThread::GameThread(const QString &rootDirPath, QObject *parent):
    QThread(parent),
    m_rootDirPath(rootDirPath)
{
}

GameThread::~GameThread(void)
{
}

void GameThread::run(void)
{
    QScopedPointer<Game::ContextImpl> context(new Game::ContextImpl());
    if (!context || !context->init(m_rootDirPath))
        return;

    QElapsedTimer elapsedTimer;
    elapsedTimer.start();

    for (;;) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 16);
        context->update(float(elapsedTimer.restart()));
    }
}

VOLCANO_LAUNCHER_END
