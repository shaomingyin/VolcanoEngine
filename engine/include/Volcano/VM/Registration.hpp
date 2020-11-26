//
//
#ifndef VOLCANO_VM_REGISTRATION_HPP
#define VOLCANO_VM_REGISTRATION_HPP

#include <string>

#include <Volcano/VM/Common.hpp>

VOLCANO_VM_BEGIN

class Registration final: public Noncopyable {
public:
    Registration(lua_State *L, const char *name = nullptr);
    Registration(Registration &&that);
    virtual ~Registration(void);

public:
    lua_State *state(void);
    const std::string &name(void) const;
    Registration table(const char *name);
    Registration &def(const char *name, bool v);
    Registration &def(const char *name, int v);
    Registration &def(const char *name, float v);
    Registration &def(const char *name, double v);
    Registration &def(const char *name, const char *p);
    Registration &def(const char *name, lua_CFunction fn);
    Registration &def(const char *name, rttr::type type);
    Registration &operator=(Registration &&that);

protected:
    bool isNameExists(const char *name);

protected:
    lua_State *m_L;
    std::string m_name;
};

VOLCANO_INLINE lua_State *Registration::state(void)
{
    return m_L;
}

VOLCANO_INLINE const std::string &Registration::name(void) const
{
    VOLCANO_ASSERT(m_L != nullptr);

    return m_name;
}

VOLCANO_INLINE Registration Registration::table(const char *name)
{
    VOLCANO_ASSERT(name != nullptr);

    return Registration(m_L, name);
}

VOLCANO_INLINE Registration &Registration::operator=(Registration &&that)
{
    if (this != &that) {
        m_L = that.m_L;
        m_name = std::move(that.m_name);
        that.m_L = nullptr;
    }

    return *this;
}

VOLCANO_INLINE bool Registration::isNameExists(const char *name)
{
    lua_getfield(m_L, -1, name);
    bool ret = !lua_isnil(m_L, -1);
    lua_pop(m_L, 1);
    return ret;
}

VOLCANO_VM_END

#endif // VOLCANO_VM_REGISTRATION_HPP
