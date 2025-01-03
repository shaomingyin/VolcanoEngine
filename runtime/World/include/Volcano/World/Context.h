//
//
#ifndef VOLCANO_WORLD_CONTEXT_H
#define VOLCANO_WORLD_CONTEXT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(QObject* parent = nullptr);

public:
    virtual QNetworkAccessManager* networkAccessManager(QObject* object);
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CONTEXT_H
