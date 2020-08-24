//
//
#ifndef VOLCANO_NODE_HPP
#define VOLCANO_NODE_HPP

#include <QList>
#include <QString>
#include <QQmlListProperty>
#include <QObject>
#include <QWindow>

#include <Volcano/Common.hpp>
#include <Volcano/OpenGL/View.hpp>

VOLCANO_BEGIN

class Node;
typedef QList<Node *> NodeList;

class VOLCANO_API Node: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QObject *parent READ parentNode NOTIFY parentNodeChanged)
    Q_PROPERTY(QQmlListProperty<Volcano::Node> subNodes READ qmlSubNodes)
    Q_CLASSINFO("DefaultProperty", "subNodes")

public:
    Node(Node *parent = nullptr);
    ~Node(void) override;

public:
    const QString &name(void) const;
    void setName(const QString &v);
    bool enabled(void) const;
    void setEnabled(bool v);
    bool visibled(void) const;
    void setVisibled(bool v);
    Node *parentNode(void);
    QQmlListProperty<Volcano::Node> qmlSubNodes(void);
    const NodeList &subNodes(void) const;
    QWindow *window(void) const;
    void update(float elapsed, bool recursive = true);
    void draw(OpenGL::View &view, bool recursive = true);
    void setWindow(QWindow *window);

signals:
    void nameChanged(const QString &name);
    void windowChanged(QWindow *window);
    void parentNodeChanged(Node *parent);

protected:
    bool event(QEvent *evt) override;
    virtual void onUpdate(float elapsed);
    virtual void onDraw(OpenGL::View &view);

private:
    void setParentNode(Node *parent);
    static void qmlAppendSubNode(QQmlListProperty<Volcano::Node> *nodes, Node *node);
    static int qmlSubNodeCount(QQmlListProperty<Volcano::Node> *nodes);
    static Node *qmlSubNode(QQmlListProperty<Volcano::Node> *nodes, int index);
    static void qmlClearSubNode(QQmlListProperty<Volcano::Node> *nodes);
    static void qmlReplaceSubNode(QQmlListProperty<Volcano::Node> *nodes, int index, Node *node);
    static void qmlRemoveLastSubNode(QQmlListProperty<Volcano::Node> *nodes);

private:
    enum
    {
        FlagEnabled = 0x1,
        FlagVisibled = 0x2
    };

    QString m_name;
    int m_flags;
    Node *m_parentNode;
    NodeList m_subNodes;
    QWindow *m_window;
};

VOLCANO_INLINE const QString &Node::name(void) const
{
    return m_name;
}

VOLCANO_INLINE void Node::setName(const QString &v)
{
    if (m_name != v)
    {
        m_name = v;
        nameChanged(v);
    }
}

VOLCANO_INLINE bool Node::enabled(void) const
{
    return (m_flags & FlagEnabled);
}

VOLCANO_INLINE void Node::setEnabled(bool v)
{
    if (v)
        m_flags |= FlagEnabled;
    else
        m_flags &= ~FlagEnabled;
}

VOLCANO_INLINE bool Node::visibled(void) const
{
    return (m_flags & FlagVisibled);
}

VOLCANO_INLINE void Node::setVisibled(bool v)
{
    if (v)
        m_flags |= FlagVisibled;
    else
        m_flags &= ~FlagVisibled;
}

VOLCANO_INLINE Node *Node::parentNode(void)
{
    return m_parentNode;
}

VOLCANO_INLINE const NodeList &Node::subNodes(void) const
{
    return m_subNodes;
}

VOLCANO_INLINE QWindow *Node::window(void) const
{
    return m_window;
}

VOLCANO_END

#endif // VOLCANO_NODE_HPP
