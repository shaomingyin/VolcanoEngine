//
//
#ifndef VOLCANO_WORLD_ENTITY_H
#define VOLCANO_WORLD_ENTITY_H

#include <Volcano/Error.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Entity final {
public:
	Entity()
        : inherency_(nullptr) {
    }

	Entity(const Entity&) = default;
	Entity& operator=(const Entity&) = default;

public:
	[[nodiscard]] bool isValid() const noexcept {
        return (handle_.valid() && inherency_ != nullptr);
	}

    [[nodiscard]] explicit operator bool() const noexcept {
        return isValid();
    }

    void destroy() {
        VOLCANO_ASSERT(isValid());
        handle_.destroy();
    }

    template<typename Component, typename... Args>
    decltype(auto) emplace(Args &&...args) const {
        VOLCANO_ASSERT(isValid());
        return handle_.emplace<Component>(entt, std::forward<Args>(args)...);
    }

    template<typename Component, typename... Args>
    decltype(auto) emplaceOrReplace(Args &&...args) const {
        VOLCANO_ASSERT(isValid());
        return handle_.emplace_or_replace<Component>(entt, std::forward<Args>(args)...);
    }

    template<typename Component, typename... Func>
    decltype(auto) patch(Func &&...func) const {
        VOLCANO_ASSERT(isValid());
        return handle_.patch<Component>(entt, std::forward<Func>(func)...);
    }

    template<typename Component, typename... Args>
    decltype(auto) replace(Args &&...args) const {
        VOLCANO_ASSERT(isValid());
        return handle_.replace<Component>(entt, std::forward<Args>(args)...);
    }

    template<typename... Component>
    size_type remove() const {
        VOLCANO_ASSERT(isValid());
        return handle_.remove<Component...>(entt);
    }

    template<typename... Component>
    void erase() const {
        VOLCANO_ASSERT(isValid());
        handle_.erase<Component...>(entt);
    }

    template<typename... Component>
    [[nodiscard]] decltype(auto) allOf() const {
        VOLCANO_ASSERT(isValid());
        return handle_.all_of<Component...>(entt);
    }

    template<typename... Component>
    [[nodiscard]] decltype(auto) anyOf() const {
        VOLCANO_ASSERT(isValid());
        return handle_.any_of<Component...>(entt);
    }

    template<typename... Component>
    [[nodiscard]] decltype(auto) get() const {
        VOLCANO_ASSERT(isValid());
        return handle_.get<Component...>(entt);
    }

    template<typename Component, typename... Args>
    [[nodiscard]] decltype(auto) getOrEmplace(Args &&...args) const {
        VOLCANO_ASSERT(isValid());
        return handle_.get_or_emplace<Component>(entt, std::forward<Args>(args)...);
    }

    template<typename... Component>
    [[nodiscard]] auto tryGet() const {
        VOLCANO_ASSERT(isValid());
        return handle_.try_get<Component...>(entt);
    }

    [[nodiscard]] bool orphan() const {
        VOLCANO_ASSERT(isValid());
        return handle_.orphan();
    }

public:
    bool isEnabled() const {
        if (handle_.valid() && inherency_ != nullptr) {
            return bool(inherency_->flags & Inherency::FlagEnabled);
        }
        return false;
    }

    bool isVisible() const {
        if (isEnabled()) {
            return bool(inherency_->flags & Inherency::FlagVisible);
        }
        return false;
    }

    const std::string& name() {
        VOLCANO_ASSERT(isValid());
        return inherency_->name;
    }

    Eigen::Affine3f& transform() {
        VOLCANO_ASSERT(isValid());
        return inherency_->transform;
    }

    const Eigen::Affine3f& transform() const {
        VOLCANO_ASSERT(isValid());
        return inherency_->transform;
    }

private:
	friend class Scene;

    struct Inherency {
        enum {
            FlagEnabled = 0x1,
            FlagVisible = 0x2
        };

        int flags;
        std::string name;
        Eigen::Affine3f transform;

        Inherency(std::string n)
            : flags(FlagEnabled | FlagVisible)
            , name(std::move(n))
            , transform(Eigen::Affine3f::Identity()) {
        }
    };

    Entity(entt::registry& reg, entt::entity ent, std::string name = std::string())
        : handle_(reg, ent)
        , inherency_(&reg.get_or_emplace<Inherency>(ent, std::move(name))) {
        if (inherency_ == nullptr) {
            throw Error(Errc::OutOfResource);
        }
    }

private:
	entt::handle handle_;
    Inherency* inherency_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_ENTITY_H
