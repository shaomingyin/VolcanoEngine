//
//
#ifndef VOLCANO_LAUNCHER_GAMETHREAD_HPP
#define VOLCANO_LAUNCHER_GAMETHREAD_HPP

#include <QUrl>
#include <QThread>
#include <QAtomicInt>

#include <Volcano/Launcher/Common.hpp>
#include <Volcano/Launcher/GameContext.hpp>

VOLCANO_LAUNCHER_BEGIN

class GameThread: public QThread {
    Q_OBJECT

public:
    GameThread(QUrl url, QObject* parent = nullptr);

public:

protected:
    void run() override;

private:
    QUrl url_;
    GameContext game_context_;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_GAMETHREAD_HPP
