//
//
#ifndef VOLCANO_SYSTEM_COMMON_H
#define VOLCANO_SYSTEM_COMMON_H

#include <nlohmann_json.h>
#include <physfs.h>
#include <enet/enet.h>
#include <GL/gl3w.h>
#include <SDL.h>
#include <imgui.h>

#include <Volcano/Common.h>
#include <Volcano/System/Config.h>

#define VOLCANO_SYSTEM_BEGIN VOLCANO_BEGIN namespace System {
#define VOLCANO_SYSTEM_END } VOLCANO_END

#define VOLCANO_SYSTEM_DEFAULT_PORT 6783

#endif // VOLCANO_SYSTEM_COMMON_H
