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
    Q_PROPERTY(Node *parentNode READ parentNode NOTIFY parentChanged)
    Q_PROPERTY(QQmlListProperty<Volcano::Node> subNodes READ subNodes)
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
    QQmlListProperty<Volcano::Node> subNodes(void);
    const Nodes &subNodesRO(void) const;

signals:
    void parentChanged(Node *parent);

private:
    void setParentNode(Node *parent);
    static void addSubNode(QQmlListProperty<Volcano::Node> *nodes, Node *node);
    static int subNodeCount(QQmlListProperty<Volcano::Node> *nodes);
    static Node *subNode(QQmlListProperty<Volcano::Node> *nodes, int index);
    static void clearSubNode(QQmlListProperty<Volcano::Node> *nodes);
    static void replaceSubNode(QQmlListProperty<Volcano::Node> *nodes, int index, Node *node);
    static void removeLastSubNode(QQmlListProperty<Volcano::Node> *nodes);

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
        parentChanged(parent);
    }
}

VOLCANO_INLINE const Node::Nodes &Node::subNodesRO(void) const
{
    return m_subNodes;
}

VOLCANO_END

#endif // VOLCANO_NODE_HPP
