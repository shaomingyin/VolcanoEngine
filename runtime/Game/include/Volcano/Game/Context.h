//
//
#ifndef VOLCANO_GAME_CONTEXT_H
#define VOLCANO_GAME_CONTEXT_H

#include <QObject>
#include <QQmlEngine>
#include <QNetworkAccessManager>

#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(QObject* parent = nullptr);

public:
    virtual QNetworkAccessManager* networkAccessManager() = 0;

    void attachToQmlEngine(QQmlEngine& qml_engine);
    static Context* fromQmlEngine(QQmlEngine& qml_engine);

private:
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXT_H
