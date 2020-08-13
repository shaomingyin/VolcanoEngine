//
//
#ifndef VOLCANO_NODE_HPP
#define VOLCANO_NODE_HPP

#include <QVector>
#include <QObject>
#include <QQmlListProperty>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API Node: public QObject
{
    Q_OBJECT
    Q_PROPERTY(Node *parent READ parentNode NOTIFY parentNodeChanged)
    Q_PROPERTY(QQmlListProperty<Volcano::Node> subNodes READ qmlSubNodes)
    Q_CLASSINFO("DefaultProperty", "subNodes")

public:
    typedef QVector<Node *> Nodes;

public:
	Node(Node *parent = nullptr);
    ~Node(void) override;

public:
	bool enabled(void) const;
	void setEnabled(bool v);
	bool visibled(void) const;
	void setVisibled(bool v);
    Node *parentNode(void);
    QQmlListProperty<Volcano::Node> qmlSubNodes(void);
    const Nodes &subNodes(void) const;
    void tick(float elapsed, bool recursive = true);

signals:
    void parentNodeChanged(Node *parent);

protected:
    virtual void onTick(float elapsed);

private:
    void setParentNode(Node *parent);
    static void qmlAddSubNode(QQmlListProperty<Volcano::Node> *nodes, Node *node);
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

	int m_flags;
    Node *m_parentNode;
    Nodes m_subNodes;
};

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

VOLCANO_INLINE void Node::setParentNode(Node *parent)
{
    if (m_parentNode != parent)
    {
        m_parentNode = parent;
        parentNodeChanged(parent);
    }
}

VOLCANO_INLINE const Node::Nodes &Node::subNodes(void) const
{
    return m_subNodes;
}

VOLCANO_END

#endif // VOLCANO_NODE_HPP
