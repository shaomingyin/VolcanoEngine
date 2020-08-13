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

QQmlListProperty<Volcano::Node> Node::qmlSubNodes(void)
{
    return QQmlListProperty<Volcano::Node>(this, this,
             &Node::qmlAddSubNode,
             &Node::qmlSubNodeCount,
             &Node::qmlSubNode,
             &Node::qmlClearSubNode,
             &Node::qmlReplaceSubNode,
             &Node::qmlRemoveLastSubNode);
}

void Node::tick(float elapsed, bool recursive)
{
    if (recursive)
    {
        for (auto it(m_subNodes.begin()); it != m_subNodes.end(); ++it)
            (*it)->tick(elapsed, true);
    }

    onTick(elapsed);
}

void Node::onTick(float elapsed)
{
}

void Node::qmlAddSubNode(QQmlListProperty<Volcano::Node> *nodes, Node *node)
{
    Node *parent = reinterpret_cast<Node *>(nodes->data);
    parent->m_subNodes.append(node);
    node->setParentNode(parent);
}

int Node::qmlSubNodeCount(QQmlListProperty<Volcano::Node> *nodes)
{
    return reinterpret_cast<Node *>(nodes->data)->m_subNodes.size();
}

Node *Node::qmlSubNode(QQmlListProperty<Volcano::Node> *nodes, int index)
{
    return reinterpret_cast<Node *>(nodes->data)->m_subNodes.at(index);
}

void Node::qmlClearSubNode(QQmlListProperty<Volcano::Node> *nodes)
{
    reinterpret_cast<Node *>(nodes->data)->m_subNodes.clear();
}

void Node::qmlReplaceSubNode(QQmlListProperty<Volcano::Node> *nodes, int index, Node *node)
{
    reinterpret_cast<Node *>(nodes->data)->m_subNodes[index] = node;
}

void Node::qmlRemoveLastSubNode(QQmlListProperty<Volcano::Node> *nodes)
{
    reinterpret_cast<Node *>(nodes->data)->m_subNodes.removeLast();
}

VOLCANO_END
