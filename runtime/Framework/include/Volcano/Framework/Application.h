/*
 *
 */
#ifndef VOLCANO_FRAMEWORK_APPLICATION_H
#define VOLCANO_FRAMEWORK_APPLICATION_H

#include <string>

#include <nlohmann_json.h>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>

#include <Volcano/VersionNumber.h>
#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

template <typename T>
concept Application_construct = requires(Context& context) {
    T(context);
};

template <typename T>
concept Application_loadConfig = requires(T & v, const nlohmann::json & j) {
    v.loadConfig(j);
};

template <typename T>
concept Application_loadScene = requires(T & v, const nlohmann::json & j) {
    v.loadScene(j);
};

template <typename T>
concept Application_event = requires(T & v, const SDL_Event & evt) {
    v.event(evt);
};

template <typename T>
concept Application_frame = requires(T & v, Duration elapsed) {
    v.frame(elapsed);
};

template <typename T>
concept Application =
    Application_construct<T> &&
    Application_loadConfig<T> &&
    Application_loadScene<T> &&
    Application_event<T> &&
    Application_frame<T>;

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_APPLICATION_H
