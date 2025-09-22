//
//
#ifndef VOLCANO_WORLD_LOADABLE_H
#define VOLCANO_WORLD_LOADABLE_H

#include <QUrl>
#include <QNetworkReply>

#include <Volcano/World/Common.h>
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

class Loadable: public Transformable {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged FINAL)

public:
    Loadable(QObject* parent = nullptr);

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

private:
    QUrl source_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_LOADABLE_H
