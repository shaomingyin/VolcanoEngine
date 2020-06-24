//
//
#include <Volcano/Node.h>

VOLCANO_BEGIN

Node::Node(QObject *parent):
    QObject(parent),
    m_flags(FlagEnable | FlagVisible),
    m_state(StateLoading),
    m_progress(0.0),
    m_parentNode(nullptr)
{
}

Node::~Node(void)
{
}

QQmlListProperty<Node> Node::subNodesProperty(void)
{
    return QQmlListProperty<Node>(this, this,
             &Node::addSubNode,
             &Node::subNodeCount,
             &Node::subNode,
             &Node::clearSubNodes);
}

QMatrix4x4 Node::globalTransform(void)
{
    QMatrix4x4 m(transform());

    Node *obj = this;
    for (;;)
    {
        Node *parent = obj->parentNode();
        if (parent == nullptr)
            break;
        m = parent->transform() * m;
        obj = obj->parentNode();
    }

    return m;
}

void Node::setState(State v)
{
    if (m_state != v)
    {
        m_state = v;
        stateChanged(v);
    }
}

void Node::setProgress(qreal v)
{
    if (qAbs(m_progress - v) > 0.001)
    {
        m_progress = v;
        progressChanged(v);
    }
}

void Node::setParentNode(Node *node)
{
    if (m_parentNode != node)
    {
        m_parentNode = node;
        parentNodeChanged(node);
    }
}

void Node::addSubNode(QQmlListProperty<Node> *property, Node *value)
{
    Node *node = reinterpret_cast<Node *>(property->data);
    value->setParent(node);
    value->setParentNode(node);
    node->m_subNodes.append(value);
}

int Node::subNodeCount(QQmlListProperty<Node> *property)
{
    return reinterpret_cast<Node *>(property->data)->m_subNodes.size();
}

Node *Node::subNode(QQmlListProperty<Node> *property, int index)
{
    return reinterpret_cast<Node *>(property->data)->m_subNodes.at(index);
}

void Node::clearSubNodes(QQmlListProperty<Node> *property)
{
    Node *node = reinterpret_cast<Node *>(property->data);
    for (auto it: node->m_subNodes)
        it->setParentNode(nullptr);
    node->m_subNodes.clear();
}

#if 0

void Node::replaceSubObjectFunc(QQmlListProperty<Node> *property, int index, Node *value)
{
    reinterpret_cast<Node *>(property->data)->m_subNodes.replace(index, value);
}

void Node::removeLastSubObjectFunc(QQmlListProperty<Node> *property)
{
    reinterpret_cast<Node *>(property->data)->m_subNodes.removeLast();
}

#endif

VOLCANO_END
