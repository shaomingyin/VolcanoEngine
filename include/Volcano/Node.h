//
//
#ifndef VOLCANO_NODE_H
#define VOLCANO_NODE_H

#include <QList>
#include <QObject>
#include <QString>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QQmlListProperty>

#include <Volcano/Common.h>

VOLCANO_BEGIN

class Node: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enable READ enable WRITE setEnable NOTIFY enableChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(Node *parent READ parentNode NOTIFY parentNodeChanged)
    Q_PROPERTY(QQmlListProperty<Volcano::Node> children READ subNodesProperty)
    Q_CLASSINFO("DefaultProperty", "children")

public:
    enum State
    {
        StateLoading = 0,
        StateReady,
        StateError
    };
    Q_ENUMS(State)

    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)

public:
    Node(QObject *parent = nullptr);
    virtual ~Node(void);

public:
    bool enable(void) const;
    void setEnable(bool v);
    bool visible(void) const;
    void setVisible(bool v);
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    const QVector3D &scale(void) const;
    void setScale(const QVector3D &v);
    const QQuaternion &rotation(void) const;
    void setRotation(const QQuaternion &v);
    Node *parentNode(void);
    QQmlListProperty<Node> subNodesProperty(void);
    const QList<Node *> &subNodes(void) const;
    const QMatrix4x4 &transform(void);
    QMatrix4x4 globalTransform(void);
    State state(void) const;
    qreal progress(void) const;

signals:
    void enableChanged(bool v);
    void visibleChanged(bool v);
    void positionChanged(const QVector3D &v);
    void scaleChanged(const QVector3D &v);
    void rotationChanged(const QQuaternion &v);
    void parentNodeChanged(Node *p);
    void stateChanged(State v);
    void progressChanged(qreal v);

protected:
    void setState(State v);
    void setProgress(qreal v);
    void setParentNode(Node *node);
    static void addSubNode(QQmlListProperty<Node> *property, Node *value);
    static int subNodeCount(QQmlListProperty<Node> *property);
    static Node *subNode(QQmlListProperty<Node> *property, int index);
    static void clearSubNodes(QQmlListProperty<Node> *property);
#if 0
    static void replaceSubNode(QQmlListProperty<Node> *property, int index, Node *value);
    static void removeLastSubNode(QQmlListProperty<Node> *property);
#endif

private:
    enum
    {
        FlagEnable = 0x1,
        FlagVisible = 0x2,
        FlagDirty = 0x4
    };

    int m_flags;
    State m_state;
    qreal m_progress;
    QVector3D m_position;
    QVector3D m_scale;
    QQuaternion m_rotation;
    QMatrix4x4 m_transform;
    Node *m_parentNode;
    QList<Node *> m_subNodes;
};

VOLCANO_INLINE bool Node::enable(void) const
{
    return (m_flags & FlagEnable);
}

VOLCANO_INLINE void Node::setEnable(bool v)
{
    if (enable() != v)
    {
        if (v)
            m_flags |= FlagEnable;
        else
            m_flags &= ~FlagEnable;
        enableChanged(v);
    }
}

VOLCANO_INLINE bool Node::visible(void) const
{
    return (m_flags & FlagVisible);
}

VOLCANO_INLINE void Node::setVisible(bool v)
{
    if (visible() != v)
    {
        if (v)
            m_flags |= FlagVisible;
        else
            m_flags &= ~FlagVisible;

        visibleChanged(v);
    }
}

VOLCANO_INLINE const QVector3D &Node::position(void) const
{
    return m_position;
}

VOLCANO_INLINE void Node::setPosition(const QVector3D &v)
{
    if (m_position != v)
    {
        m_position = v;
        m_flags |= FlagDirty;
        positionChanged(v);
    }
}

VOLCANO_INLINE const QVector3D &Node::scale(void) const
{
    return m_scale;
}

VOLCANO_INLINE void Node::setScale(const QVector3D &v)
{
    if (m_scale != v)
    {
        m_scale = v;
        m_flags |= FlagDirty;
        scaleChanged(v);
    }
}

VOLCANO_INLINE const QQuaternion &Node::rotation(void) const
{
    return m_rotation;
}

VOLCANO_INLINE void Node::setRotation(const QQuaternion &v)
{
    if (m_rotation != v)
    {
        m_rotation = v;
        m_flags |= FlagDirty;
        rotationChanged(v);
    }
}

VOLCANO_INLINE Node *Node::parentNode(void)
{
    return m_parentNode;
}

VOLCANO_INLINE const QList<Node *> &Node::subNodes(void) const
{
    return m_subNodes;
}

VOLCANO_INLINE const QMatrix4x4 &Node::transform(void)
{
    if (m_flags & FlagDirty)
    {
        m_transform.setToIdentity();
        m_transform.scale(scale());
        m_transform.rotate(rotation());
        m_transform.translate(position());
        m_flags &= ~FlagDirty;
    }
    return m_transform;
}

VOLCANO_INLINE Node::State Node::state(void) const
{
    return m_state;
}

VOLCANO_INLINE qreal Node::progress(void) const
{
    if (m_state == StateLoading)
        return m_progress;
    if (m_state == StateReady)
        return 1.0;
    return 0.0;
}

VOLCANO_END

#endif // VOLCANO_NODE_H
