//
//
#ifndef VOLCANO_RENDERER_MESH_H
#define VOLCANO_RENDERER_MESH_H

#include <QUrl>

#include <Volcano/Node.h>
#include <Volcano/Renderer/Common.h>

VOLCANO_RENDERER_BEGIN

class VOLCANO_API Mesh: public Node
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

public:
    Mesh(QObject *parent = nullptr);
    ~Mesh(void) override;

public:
    const QUrl &source(void) const;
    void setSource(const QUrl &v);

signals:
    void sourceChanged(const QUrl &v);

private:
    QUrl m_source;
};

VOLCANO_INLINE const QUrl &Mesh::source(void) const
{
    return m_source;
}

VOLCANO_RENDERER_END

#endif // VOLCANO_RENDERER_MESH_H
