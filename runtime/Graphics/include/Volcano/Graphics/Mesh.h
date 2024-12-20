//
//
#ifndef VOLCANO_GRAPHICS_MESH_H
#define VOLCANO_GRAPHICS_MESH_H

#include <QUrl>

#include <Volcano/World/Transformable.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Mesh: public World::Transformable {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged FINAL)

public:
    Mesh(QObject* parent = nullptr);
    virtual ~Mesh();

public:
    const QUrl& source() const {
        return source_;
    }

    void setSource(const QUrl& v);

signals:
    void sourceChanged(const QUrl& v);

private:
    QUrl source_;
	unsigned int color_channels_;
	unsigned int uv_channels_;
	bool has_normals_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MESH_H
