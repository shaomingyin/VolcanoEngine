//
//
#ifndef VOLCANO_VM_OBJECT_HPP
#define VOLCANO_VM_OBJECT_HPP

#include <string>

#include <Volcano/Graphics/View.hpp>
#include <Volcano/VM/Common.hpp>

VOLCANO_VM_BEGIN

class Object: public Noncopyable {
public:
    Object(void);
    virtual ~Object(void);

public:
    const std::string &name(void) const;
    void setName(const std::string &name);
    bool isEnabled(void) const;
    void enable(void);
    void disable(void);
    virtual void onUpdate(float elapsed);
    virtual bool onEvent(const SDL_Event &evt);
    virtual void applyToView(Graphics::View &view);

protected:
    virtual void onNameChanged(const std::string &name);
    virtual void onEnabled(void);
    virtual void onDisabled(void);

private:
    enum {
        FLAG_ENABLED = 0x1
    };

private:
    std::string m_name;
    int m_flags;
};

VOLCANO_INLINE const std::string &Object::name(void) const
{
    return m_name;
}

VOLCANO_INLINE void Object::setName(const std::string &name)
{
    if (m_name != name) {
        m_name = name;
        onNameChanged(name);
    }
}

VOLCANO_INLINE bool Object::isEnabled(void) const
{
    return m_flags & FLAG_ENABLED;
}

VOLCANO_VM_END

#endif // VOLCANO_VM_OBJECT_HPP
