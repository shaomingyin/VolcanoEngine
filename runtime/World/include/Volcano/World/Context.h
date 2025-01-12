//
//
#ifndef VOLCANO_WORLD_CONTEXT_H
#define VOLCANO_WORLD_CONTEXT_H

#include <memory>

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
    QNetworkAccessManager* networkAccessManager(QObject* object);

protected:
    virtual QNetworkAccessManager* createFallbackNetworkAccessManager();

private:
    std::unique_ptr<QNetworkAccessManager> fallback_network_access_manager_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CONTEXT_H
