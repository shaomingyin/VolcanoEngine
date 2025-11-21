//
//
#ifndef VOLCANO_WORLD_ENTITY_HPP
#define VOLCANO_WORLD_ENTITY_HPP

#include <map>

#include <Volcano/World/Common.hpp>
#include <Volcano/World/Transform.hpp>
#include <Volcano/World/Context.hpp>
#include <Volcano/World/Component.hpp>

VOLCANO_WORLD_BEGIN

class Entity {
    RTTR_ENABLE()

public:
    using Components = std::map<rttr::type, rttr::variant>;

public:
    Entity(Context& context)
        : context_(context)
        , flags_(0) {
    }

public:
    bool isEnable() const noexcept {
        return (flags_ & FlagEnabled);
    }

    void enable() noexcept {
        flags_ |= FlagEnabled;
    }

    void disable() noexcept {
        flags_ &= ~FlagEnabled;
    }

    bool isVisible() const noexcept {
        return (flags_ & FlagVisible);
    }

    void show() noexcept {
        flags_ |= FlagVisible;
    }

    void hide() noexcept {
        flags_ &= ~FlagVisible;
    }

    Transform& transform() noexcept {
        return transform_;
    }

    const Transform& transform() const noexcept {
        return transform_;
    }

    template <Component T, typename... Args>
    T& emplace(Args... args) {
        auto res = components_.emplace({ rttr::type::get<T>(), T(context_, args...) });
        return res.first->second.template get_value<T>();
    }

    rttr::variant& emplace(rttr::type type, std::vector<rttr::argument> args) {
        assert(type.is_base_of(rttr::type::get<ComponentBase>()));
        std::vector<rttr::argument> context_args;
        //context_args.push_back(rttr::argument(std::ref(context_)));

        auto res = components_.emplace(type, type.create(std::move(args)));
        return res.first->second;
    }

    template <Component T>
    T& get() {
        return components_.at(rttr::type::get<T>());
    }

    rttr::variant& get(rttr::type type) {
        assert(type.is_base_of(rttr::type::get<ComponentBase>()));
        return components_.at(type);
    }

    const rttr::variant& get(rttr::type type) const {
        assert(type.is_base_of(rttr::type::get<ComponentBase>()));
        return components_.at(type);
    }

    template <Component T, typename... Args>
    T& getOrEmplace(Args... args) {
        auto it = components_.find(rttr::type::get<T>());
        if (it != components_.end()) {
            return it->second;
        }
        return emplace<T>(std::forward<Args...>(args...));
    }

    template <typename... Args>
    rttr::variant& getOrEmplace(rttr::type type, Args... args) {
        assert(type.is_base_of(rttr::type::get<ComponentBase>()));
        auto it = components_.find(type);
        if (it != components_.end()) {
            return it->second;
        }
        return emplace(type, std::forward<Args...>(args...));
    }

    template <Component T>
    void remove() {
        components_.erase(rttr::type::get<T>());
    }

    void remove(rttr::type type) {
        assert(type.is_base_of(rttr::type::get<ComponentBase>()));
        components_.erase(type);
    }

    void clear() {
        components_.clear();
    }

private:
    enum {
        FlagEnabled = 0x1,
        FlagVisible = 0x2
    };

    Context& context_;
    int flags_;
    Transform transform_;
    Components components_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_ENTITY_HPP
