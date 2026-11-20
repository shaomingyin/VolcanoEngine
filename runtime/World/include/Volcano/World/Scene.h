//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <memory>
#include <string>

#include <entt/entt.hpp>

#include <Volcano/Version.h>
#include <Volcano/World/Common.h>
#include <Volcano/World/Context.h>

VOLCANO_WORLD_BEGIN

class Scene : public entt::registry {
public:
    explicit Scene(Context& context);
    Scene();
    virtual ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = default;
    Scene& operator=(Scene&&) = default;

public:
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
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
