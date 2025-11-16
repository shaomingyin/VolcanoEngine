//
//
#ifndef VOLCANO_VM_COMMON_HPP
#define VOLCANO_VM_COMMON_HPP

#include <string>

#include <physfs.h>
#include <uv.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Volcano/Common.hpp>

#define VOLCANO_VM_BEGIN VOLCANO_BEGIN namespace Vm {
#define VOLCANO_VM_END } VOLCANO_END

VOLCANO_VM_BEGIN

class GlobalTable final {
public:
	GlobalTable(lua_State* L, const char* name)
		: state_(L)
		, name_(name) {
		lua_getglobal(L, name);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			lua_newtable(L);
			new_ = true;
		} else {
			new_ = false;
		}
	}

	~GlobalTable() {
		if (new_) {
			lua_setglobal(state_, name_.c_str());
		} else {
			lua_pop(state_, 1);
		}
	}

private:
	lua_State* state_;
	std::string name_;
	bool new_;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_COMMON_HPP
