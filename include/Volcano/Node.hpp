//
//
#ifndef VOLCANO_NODE_HPP
#define VOLCANO_NODE_HPP

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Node {
public:
	Node(Node *parent = nullptr);
	virtual ~Node(void);

public:
	bool enabled(void) const;
	void setEnabled(bool v);
	bool visibled(void) const;
	void setVisibled(bool v);
	Node *parent(void);
	void setParent(Node *newParent);
	Node *firstChild(void);
	Node *prevSibling(void);
	Node *nextSibling(void);

private:
	enum {
		FlagEnabled = 0x1,
		FlagVisibled = 0x2
	};

	int m_flags;
	Node *m_parent;
	Node *m_firstChild;
	Node *m_prevSibling;
	Node *m_nextSibling;
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

VOLCANO_INLINE Node *Node::parent(void)
{
	return m_parent;
}

VOLCANO_INLINE Node *Node::firstChild(void)
{
	return m_firstChild;
}

VOLCANO_INLINE Node *Node::prevSibling(void)
{
	return m_prevSibling;
}

VOLCANO_INLINE Node *Node::nextSibling(void)
{
	return m_nextSibling;
}

VOLCANO_END

#endif // VOLCANO_NODE_HPP
