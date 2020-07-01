//
//
#include <Volcano/Node.hpp>

VOLCANO_BEGIN

Node::Node(Node *parent) :
	m_flags(FlagEnabled | FlagVisibled)
{
	setParent(parent);
}

Node::~Node(void)
{
	setParent(nullptr);
}

void Node::setParent(Node *newParent)
{
	if (m_parent != nullptr) {
		if (m_prevSibling != nullptr)
			m_prevSibling->m_nextSibling = m_nextSibling;
		else
			m_parent->m_firstChild = m_nextSibling;
		if (m_nextSibling != nullptr)
			m_nextSibling->m_prevSibling = m_prevSibling;
	}

	m_parent = newParent;
	m_prevSibling = nullptr;

	if (m_parent != nullptr) {
		m_nextSibling = m_parent->m_firstChild;
		m_parent->m_firstChild = this;
	} else
		m_nextSibling = nullptr;
}

VOLCANO_END
