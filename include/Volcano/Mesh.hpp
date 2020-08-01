//
//
#ifndef VOLCANO_MESH_HPP
#define VOLCANO_MESH_HPP

#include <QString>

#include <Volcano/Common.hpp>
#include <Volcano/Node.hpp>

VOLCANO_BEGIN

class Mesh: public Node
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    Mesh(Node *parent = nullptr);
    ~Mesh(void) override;

public:
    const QString &source(void) const;
    void setSource(const QString &v);

signals:
    void sourceChanged(void);

private:
    QString m_source;
};

VOLCANO_INLINE const QString &Mesh::source(void) const
{
    return m_source;
}

VOLCANO_INLINE void Mesh::setSource(const QString &v)
{
    if (m_source != v)
    {
        m_source = v;
        sourceChanged();
    }
}

VOLCANO_END

#endif // VOLCANO_MESH_HPP
