//
//
#ifndef VOLCANO_WORLD_RESOURCE_H
#define VOLCANO_WORLD_RESOURCE_H

#include <QUrl>

#include <Volcano/World/Common.h>
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

class Resource: public Transformable {
    Q_OBJECT
    Q_PROPERTY(State state READ state NOTIFY stateChanged)

public:
    Q_INVOKABLE Resource(QObject* parent = nullptr);

public:
    const QUrl& source() const {
        return source_;
    }

    void setSource(const QUrl& v) {
        if (source_ != v) {
            source_ = v;
            emit sourceChanged(v);
        }
    }

    QNetworkReply* requestContent();

signals:
    void sourceChanged(const QUrl& v);

protected:
    QNetworkAccessManager* networkAccessManager();

private:
    QUrl source_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_RESOURCE_H
