//
//
#include <QQmlEngine>
#include <QQmlContext>

#include <Volcano/Node.hpp>

VOLCANO_BEGIN

Node::Node(Node *parent):
    QObject(parent),
	m_flags(FlagEnabled | FlagVisibled)
{
}

Node::~Node(void)
{
    setParentNode(nullptr);
}

QQmlListProperty<Volcano::Node> Node::subNodes(void)
{
    return QQmlListProperty<Volcano::Node>(this, this,
             &Node::addSubNode,
             &Node::subNodeCount,
             &Node::subNode,
             &Node::clearSubNode,
             &Node::replaceSubNode,
             &Node::removeLastSubNode);
}

void Node::addSubNode(QQmlListProperty<Volcano::Node> *nodes, Node *node)
{
    Node *parent = reinterpret_cast<Node *>(nodes->data);
    parent->m_subNodes.append(node);
    node->setParentNode(parent);
}

int Node::subNodeCount(QQmlListProperty<Volcano::Node> *nodes)
{
    return reinterpret_cast<Node *>(nodes->data)->m_subNodes.size();
}

Node *Node::subNode(QQmlListProperty<Volcano::Node> *nodes, int index)
{
    return reinterpret_cast<Node *>(nodes->data)->m_subNodes.at(index);
}

void Node::clearSubNode(QQmlListProperty<Volcano::Node> *nodes)
{
    reinterpret_cast<Node *>(nodes->data)->m_subNodes.clear();
}

void Node::replaceSubNode(QQmlListProperty<Volcano::Node> *nodes, int index, Node *node)
{
    reinterpret_cast<Node *>(nodes->data)->m_subNodes[index] = node;
}

void Node::removeLastSubNode(QQmlListProperty<Volcano::Node> *nodes)
{
    reinterpret_cast<Node *>(nodes->data)->m_subNodes.removeLast();
}

VOLCANO_END
