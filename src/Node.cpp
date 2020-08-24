//
//
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>

#include <Volcano/Node.hpp>

VOLCANO_BEGIN

Node::Node(Node *parent):
    QObject(parent),
    m_flags(FlagEnabled | FlagVisibled),
    m_parentNode(nullptr)
{
}

Node::~Node(void)
{
}

QQmlListProperty<Volcano::Node> Node::qmlSubNodes(void)
{
    return QQmlListProperty<Volcano::Node>(this, this,
             &Node::qmlAppendSubNode,
             &Node::qmlSubNodeCount,
             &Node::qmlSubNode,
             &Node::qmlClearSubNode,
             &Node::qmlReplaceSubNode,
             &Node::qmlRemoveLastSubNode);
}

void Node::update(float elapsed, bool recursive)
{
    if (recursive)
    {
        //for (auto it(m_subNodes.begin()); it != m_subNodes.end(); ++it)
        //    (*it)->update(elapsed, true);
    }

    onUpdate(elapsed);
}

void Node::draw(OpenGL::View &view, bool recursive)
{
    if (recursive)
    {
        //for (auto it(m_subNodes.begin()); it != m_subNodes.end(); ++it)
        //    (*it)->draw(elapsed, true);
    }

    onDraw(view);
}

bool Node::event(QEvent *evt)
{
#if 0
    switch (evt->type())
    {

    }
#endif

    return QObject::event(evt);
}

void Node::onUpdate(float elapsed)
{
}

void Node::onDraw(OpenGL::View &view)
{
}

void Node::setWindow(QWindow *window)
{
    if (m_window == window)
        return;

    m_window = window;
    windowChanged(window);

    for (auto it(m_subNodes.begin()); it != m_subNodes.end(); ++it)
        (*it)->setWindow(window);
}

void Node::setParentNode(Node *parent)
{
    if (m_parentNode == parent)
        return;

    setWindow(parent->window());

    m_parentNode = parent;
    parentNodeChanged(parent);
    setParent(parent);
}

void Node::qmlAppendSubNode(QQmlListProperty<Volcano::Node> *nodes, Node *node)
{
    Node *parent = reinterpret_cast<Node *>(nodes->data);
    parent->m_subNodes.append(node);
    Node *n = qobject_cast<Node *>(node);
    if (n != nullptr)
        n->setParentNode(parent);
}

int Node::qmlSubNodeCount(QQmlListProperty<Volcano::Node> *nodes)
{
    Node *p = reinterpret_cast<Node *>(nodes->data);
    return p->m_subNodes.size();
}

Node *Node::qmlSubNode(QQmlListProperty<Volcano::Node> *nodes, int index)
{
    Node *p = reinterpret_cast<Node *>(nodes->data);
    return p->m_subNodes.at(index);
}

void Node::qmlClearSubNode(QQmlListProperty<Volcano::Node> *nodes)
{
    Node *p = reinterpret_cast<Node *>(nodes->data);
    for (NodeList::iterator it(p->m_subNodes.begin()); it != p->m_subNodes.end(); ++it)
        (*it)->setParentNode(nullptr);
    reinterpret_cast<Node *>(nodes->data)->m_subNodes.clear();
}

void Node::qmlReplaceSubNode(QQmlListProperty<Volcano::Node> *nodes, int index, Node *node)
{
    Node *p = reinterpret_cast<Node *>(nodes->data);
    p->m_subNodes[index]->setParentNode(nullptr);
    p->m_subNodes[index] = node;
    node->setParentNode(p);
}

void Node::qmlRemoveLastSubNode(QQmlListProperty<Volcano::Node> *nodes)
{
    Node *p= reinterpret_cast<Node *>(nodes->data);
    p->m_subNodes.last()->setParentNode(nullptr);
    p->m_subNodes.removeLast();
}

VOLCANO_END
