//
//
#ifndef VOLCANO_WORLD_CONTEXT_H
#define VOLCANO_WORLD_CONTEXT_H

#include <entt/entt.hpp>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Context {
public:
    Context() = default;
    virtual ~Context() = default;

public:
    virtual void save(nlohmann::json& json) const = 0;
    virtual void load(const nlohmann::json& json) = 0;
    virtual void loadEntity(entt::handle ent, const nlohmann::json& json) = 0;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CONTEXT_H
