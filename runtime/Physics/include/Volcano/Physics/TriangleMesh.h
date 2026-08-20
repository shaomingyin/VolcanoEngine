//
//
#ifndef VOLCANO_WORLD_TRIANGLEMESH_H
#define VOLCANO_WORLD_TRIANGLEMESH_H

#include <QUrl>

#include <Volcano/World/Common.h>
#include <Volcano/World/Shape.h>

VOLCANO_WORLD_BEGIN

class TriangleMesh: public Shape {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

public:
    TriangleMesh(QObject* parent = nullptr);

public:
    const QUrl& source() const noexcept;
    void setSource(const QUrl& v) noexcept;

signals:
    void sourceChanged(const QUrl& v);

private:
    QUrl source_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRIANGLEMESH_H
