//
//
#ifndef VOLCANO_MESH_HPP
#define VOLCANO_MESH_HPP

#include <QUrl>

#include <Volcano/Common.hpp>
#include <Volcano/Node.hpp>

VOLCANO_BEGIN

class VOLCANO_API Mesh: public Node
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

public:
    Mesh(Node *parent = nullptr);
    ~Mesh(void) override;

public:
    const QUrl &source(void) const;
    void setSource(const QUrl &v);

signals:
    void sourceChanged(void);

private:
    QUrl m_source;
};

VOLCANO_INLINE const QUrl &Mesh::source(void) const
{
    return m_source;
}

VOLCANO_END

#endif // VOLCANO_MESH_HPP
