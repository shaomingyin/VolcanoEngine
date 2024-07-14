//
//
#include <QQmlEngine>

#include <Volcano/Launcher/GameThread.hpp>

VOLCANO_LAUNCHER_BEGIN

GameThread::GameThread(QUrl url, QObject* parent)
    : QThread(parent)
    , url_(std::move(url)) {
    setObjectName("gameThread");
}

void GameThread::run() {
    qInfo("Game url: %s\n", qPrintable(url_.toDisplayString()));

    QQmlEngine qml_engine;
    qml_engine.setBaseUrl(url_);
    //qml_engine.

    game_context_.attachToQmlEngine(qml_engine);

    exec();
}

VOLCANO_LAUNCHER_END
