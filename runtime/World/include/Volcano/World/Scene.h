//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <memory>
#include <string>
#include <utility>

#include <entt/entt.hpp>

#include <Volcano/Version.h>
#include <Volcano/World/Common.h>
#include <Volcano/World/Context.h>

VOLCANO_WORLD_BEGIN

class Scene {
public:
    explicit Scene(Context& context);
    Scene();
    virtual ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = default;
    Scene& operator=(Scene&&) = default;

public:
    entt::registry& registry() noexcept {
        return registry_;
    }

    const entt::registry& registry() const noexcept {
        return registry_;
    }

    entt::entity create() {
        return registry_.create();
    }

    void destroy(entt::entity entity) {
        registry_.destroy(entity);
    }

    bool valid(entt::entity entity) const noexcept {
        return registry_.valid(entity);
    }

    void clear() {
        registry_.clear();
    }

    template<typename Component>
    void clear() {
        registry_.clear<Component>();
    }

    template<typename Component, typename... Args>
    decltype(auto) emplace(entt::entity entity, Args&&... args) {
        return registry_.emplace<Component>(entity, std::forward<Args>(args)...);
    }

    template<typename Component, typename... Args>
    decltype(auto) emplace_or_replace(entt::entity entity, Args&&... args) {
        return registry_.emplace_or_replace<Component>(entity, std::forward<Args>(args)...);
    }

    template<typename Component>
    decltype(auto) get(entt::entity entity) {
        return registry_.get<Component>(entity);
    }

    template<typename Component>
    decltype(auto) get(entt::entity entity) const {
        return registry_.get<Component>(entity);
    }

    template<typename Component>
    auto* try_get(entt::entity entity) noexcept {
        return registry_.try_get<Component>(entity);
    }

    template<typename Component>
    const auto* try_get(entt::entity entity) const noexcept {
        return registry_.try_get<Component>(entity);
    }

    template<typename... Component>
    bool all_of(entt::entity entity) const noexcept {
        return registry_.all_of<Component...>(entity);
    }

    template<typename... Component>
    auto view() {
        return registry_.view<Component...>();
    }

    template<typename... Component>
    auto view() const {
        return registry_.view<Component...>();
    }

    template<typename Type>
    auto storage() {
        return registry_.storage<Type>();
    }

    template<typename Type>
    auto storage() const {
        return registry_.storage<Type>();
    }

    template<typename Component, typename Func>
    decltype(auto) patch(entt::entity entity, Func&& func) {
        return registry_.patch<Component>(entity, std::forward<Func>(func));
    }

    Context& context() noexcept {
        return context_;
    }

    const Context& context() const noexcept {
        return context_;
    }

    constexpr const std::string& name() const noexcept {
        return name_;
    }

    void setName(const std::string& v) {
        name_ = v;
    }

    constexpr const std::string& description() const noexcept {
        return description_;
    }

    void setDescription(const std::string& v) {
        description_ = v;
    }

    constexpr const VersionNumber& version() const noexcept {
        return version_;
    }

    void setVersion(const VersionNumber& v) {
        version_ = v;
    }

    void update(Clock::duration elapsed);
    virtual entt::entity player() const noexcept = 0;

protected:
    virtual void onUpdate(Clock::duration elapsed) = 0;

private:
    class NullContext final : public Context {
    public:
        void requestExit() override {}
    };

    static Context& defaultContext() noexcept;

private:
    std::string name_;
    std::string description_;
    VersionNumber version_;
    Context& context_;
    entt::registry registry_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
