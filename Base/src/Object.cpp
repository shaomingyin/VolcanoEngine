//
//
#include <Volcano/Object.h>

VOLCANO_BEGIN

Object::Object(void):
    m_flags(FlagEnabled | FlagVisible),
    m_parent(nullptr),
    m_firstChild(nullptr),
    m_prevSibling(nullptr),
    m_nextSibling(nullptr)
{
}

Object::~Object(void)
{
    setParent(nullptr);
}

void Object::setParent(Object *newParent)
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
