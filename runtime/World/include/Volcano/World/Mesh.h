//
//
#ifndef VOLCANO_WORLD_MESH_H
#define VOLCANO_WORLD_MESH_H

#include <QUrl>

#include <Volcano/World/Common.h>
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

class Mesh: public Transformable {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged FINAL)

public:
    Mesh(QObject* parent = nullptr);
    ~Mesh() override;

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

signals:
    void sourceChanged(const QUrl& v);

private:
    QUrl source_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MESH_H
