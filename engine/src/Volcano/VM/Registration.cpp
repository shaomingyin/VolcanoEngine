//
//
#include <utility>

#include <Volcano/VM/Registration.hpp>

VOLCANO_VM_BEGIN

Registration::Registration(lua_State *L, const char *name):
    m_L(L),
    m_name(name != nullptr ? name : "")
{
    VOLCANO_ASSERT(L != nullptr);

    if (name != nullptr) {
        VOLCANO_ASSERT(!isNameExists(name));
        lua_newtable(L);
    } else
        lua_pushglobaltable(L);
}

Registration::Registration(Registration &&that):
    m_L(that.m_L),
    m_name(std::move(that.m_name))
{
    that.m_L = nullptr;
}

Registration::~Registration(void)
{
    if (m_L != nullptr) {
        if (!m_name.empty())
            lua_setglobal(m_L, m_name.c_str());
        else
            lua_pop(m_L, 1);
    }
}

Registration &Registration::def(const char *name, bool v)
{
    VOLCANO_ASSERT(m_L != nullptr);
    VOLCANO_ASSERT(name != nullptr);
    VOLCANO_ASSERT(!isNameExists(name));

    lua_pushboolean(m_L, v);
    lua_setfield(m_L, -2, name);

    return *this;
}

Registration &Registration::def(const char *name, int v)
{
    VOLCANO_ASSERT(m_L != nullptr);
    VOLCANO_ASSERT(name != nullptr);
    VOLCANO_ASSERT(!isNameExists(name));

    lua_pushinteger(m_L, v);
    lua_setfield(m_L, -2, name);

    return *this;
}

Registration &Registration::def(const char *name, float v)
{
    VOLCANO_ASSERT(m_L != nullptr);
    VOLCANO_ASSERT(name != nullptr);
    VOLCANO_ASSERT(!isNameExists(name));

    lua_pushnumber(m_L, v);
    lua_setfield(m_L, -2, name);

    return *this;
}

Registration &Registration::def(const char *name, double v)
{
    VOLCANO_ASSERT(m_L != nullptr);
    VOLCANO_ASSERT(name != nullptr);
    VOLCANO_ASSERT(!isNameExists(name));

    lua_pushnumber(m_L, v);
    lua_setfield(m_L, -2, name);

    return *this;
}

Registration &Registration::def(const char *name, const char *p)
{
    VOLCANO_ASSERT(m_L != nullptr);
    VOLCANO_ASSERT(name != nullptr);
    VOLCANO_ASSERT(!isNameExists(name));

    lua_pushstring(m_L, p);
    lua_setfield(m_L, -2, name);

    return *this;
}

Registration &Registration::def(const char *name, lua_CFunction fn)
{
    VOLCANO_ASSERT(m_L != nullptr);
    VOLCANO_ASSERT(name != nullptr);
    VOLCANO_ASSERT(!isNameExists(name));

    lua_pushcfunction(m_L, fn);
    lua_setfield(m_L, -2, name);

    return *this;
}

Registration &Registration::def(const char *name, rttr::type type)
{
    VOLCANO_ASSERT(m_L != nullptr);
    VOLCANO_ASSERT(name != nullptr);
    VOLCANO_ASSERT(!isNameExists(name));

    // Register metatable.
    luaL_newmetatable(m_L, type.get_name().data());
    lua_pop(m_L, 1);

    // Register constructor.
    lua_pushcfunction(m_L, [](lua_State *L) -> int {
        return 0;
    });

    lua_setfield(m_L, -2, name);

    return *this;
}

VOLCANO_VM_END
