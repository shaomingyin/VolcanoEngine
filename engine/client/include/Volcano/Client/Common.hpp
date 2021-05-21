//
//
#ifndef VOLCANO_CLIENT_COMMON_HPP
#define VOLCANO_CLIENT_COMMON_HPP

#define NAPI_VERSION 7
#include <node_api.h>

#define NODE_ADDON_API_DISABLE_DEPRECATED
#include <napi.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <Volcano/Common.hpp>
#include <Volcano/Client/Config.hpp>

#define VOLCANO_CLIENT_BEGIN VOLCANO_BEGIN namespace Client {
#define VOLCANO_CLIENT_END } VOLCANO_END

#endif // VOLCANO_CLIENT_COMMON_HPP
