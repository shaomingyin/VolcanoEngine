//
//
#ifndef VOLCANO_WORLD_OBJECT_H
#define VOLCANO_WORLD_OBJECT_H

#include <QObject>
#include <QDataStream>
#include <QNetworkAccessManager>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Object: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged FINAL)

public:
    Object(QObject* parent = nullptr);

public:
    bool isEnabled() const {
        return enabled_;
    }

    void setEnabled(bool v) {
        if (enabled_ != v) {
            enabled_ = v;
            emit enabledChanged(v);
        }
    }

    QNetworkAccessManager* networkAccessManager();

    friend QDataStream& operator<<(QDataStream& s, const Object& v);
    friend QDataStream& operator>>(QDataStream& s, Object& v);

signals:
    void enabledChanged(bool v);

private:
    bool enabled_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_OBJECT_H
