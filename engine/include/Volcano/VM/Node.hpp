//
//
#ifndef VOLCANO_VM_NODE_HPP
#define VOLCANO_VM_NODE_HPP

#include <string>

#include <Volcano/VM/Common.hpp>

VOLCANO_VM_BEGIN

class Node {
public:
    Node(void);
    virtual ~Node(void);

public:
    const std::string &name(void) const;
    void rename(const std::string &name);
    virtual void update(float elapsed);

private:
    std::string m_name;
};

VOLCANO_INLINE const std::string &Node::name(void) const
{
    return m_name;
}

VOLCANO_INLINE void Node::rename(const std::string &name)
{
    m_name = name;
}

VOLCANO_VM_END

#endif // VOLCANO_VM_NODE_HPP
