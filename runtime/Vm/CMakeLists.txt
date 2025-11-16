
set(LUA_HEADERS
	include/luaconf.h
	include/lua.h
	include/lualib.h
	include/lauxlib.h
	include/lua.hpp
	src/lua/lapi.h
	src/lua/lcode.h
	src/lua/lctype.h
	src/lua/ldebug.h
	src/lua/ldo.h
	src/lua/lfunc.h
	src/lua/lgc.h
	src/lua/ljumptab.h
	src/lua/llex.h
	src/lua/llimits.h
	src/lua/lmem.h
	src/lua/lobject.h
	src/lua/lopcodes.h
	src/lua/lopnames.h
	src/lua/lparser.h
	src/lua/lprefix.h
	src/lua/lstate.h
	src/lua/lstring.h
	src/lua/ltable.h
	src/lua/ltm.h
	src/lua/lundump.h
	src/lua/lvm.h
	src/lua/lzio.h
)

set(LUA_SOURCES
	src/lua/lapi.c
	src/lua/lauxlib.c
	src/lua/lbaselib.c
	src/lua/lcode.c
	src/lua/lcorolib.c
	src/lua/lctype.c
	src/lua/ldblib.c
	src/lua/ldebug.c
	src/lua/ldo.c
	src/lua/ldump.c
	src/lua/lfunc.c
	src/lua/lgc.c
	src/lua/linit.c
	src/lua/liolib.c
	src/lua/llex.c
	src/lua/lmathlib.c
	src/lua/lmem.c
	src/lua/loadlib.c
	src/lua/lobject.c
	src/lua/lopcodes.c
	src/lua/loslib.c
	src/lua/lparser.c
	src/lua/lstate.c
	src/lua/lstring.c
	src/lua/lstrlib.c
	src/lua/ltable.c
	src/lua/ltablib.c
	src/lua/ltm.c
	src/lua/lundump.c
	src/lua/lutf8lib.c
	src/lua/lvm.c
	src/lua/lzio.c
)

add_library(Vm STATIC
    ${LUA_HEADERS}
    ${LUA_SOURCES}
    include/Volcano/Vm/Common.hpp
    include/Volcano/Vm/Config.hpp.in
    include/Volcano/Vm/Kernel.hpp
	include/Volcano/Vm/Task.h
	include/Volcano/Vm/Task.hpp
    src/Volcano/Vm/Common.cpp
    src/Volcano/Vm/Kernel.cpp
	src/Volcano/Vm/Kernel_syscalls.cpp
)

set_source_files_properties(${LUA_SOURCES} PROPERTIES LANGUAGE CXX)

configure_file(
    include/Volcano/Vm/Config.hpp.in
    include/Volcano/Vm/Config.hpp
)

target_include_directories(Vm PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

volcano_target_setup(Vm)
set_target_properties(Vm PROPERTIES OUTPUT_NAME VolcanoVm)

target_compile_definitions(Vm PUBLIC LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/include/Volcano/Vm/Task.h\")

target_link_libraries(Vm PUBLIC Base)

find_package(Async++)
target_link_libraries(Vm PUBLIC Async++)

find_package(sol2 REQUIRED)
target_link_libraries(Vm PUBLIC sol2::sol2)

find_package(PhysFS)
target_link_libraries(Vm PUBLIC physfs-static)

find_package(libuv)
target_link_libraries(Vm PUBLIC uv)
