//
//
#include <async++.h>
#include <edyn/edyn.hpp>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Framework/World.hpp>

VOLCANO_FRAMEWORK_BEGIN

World::World(Context& context, const std::filesystem::path& initrc)
    : Vm::Kernel(initrc)
    , context_(context)
    , state_(State::Loading)
    , gravity_(0.0f, -9.8f, 0.0f)
    , event_queue_start_(0)
    , event_queue_end_(0) {
    int ret = uv_timer_init(loop(), &frame_timer_);
    if (ret != 0) {
        throw std::runtime_error("Failed to initialize uv timer for frame");
    }
	frame_timer_.data = this;
}

World::~World() {
    // TODO loading_task_
    // edyn::detach(scene_);
    uv_close(reinterpret_cast<uv_handle_t*>(&frame_timer_), nullptr);
}

void World::play() noexcept {
	if (state_ == State::Ready) {
        edyn::set_paused(scene_, false);
    }
}

void World::pause() noexcept {
    if (state_ == State::Ready) {
        edyn::set_paused(scene_, true);
    }
}

void World::postEvent(const sf::Event& evt) noexcept {
    auto end = (event_queue_end_ + 1) & EventQueueMask;
	if (end != event_queue_start_) {
        event_queue_[event_queue_end_] = evt;
        event_queue_end_ = end;
    }
}

void World::enablePhysics() noexcept {
    physics_enabled_ = true;
    edyn::set_paused(scene_, false);
}

void World::disablePhysics() noexcept {
    physics_enabled_ = false;
    edyn::set_paused(scene_, false);
}

void World::setGravity(const Eigen::Vector3f& g) noexcept {
    gravity_ = g;
    edyn::set_gravity(scene_, { g.x(), g.y(), g.z() });
}

void World::setFpsMax(unsigned long v) noexcept {
    Clock::duration elapsed_min;
	if (v == 0) {
        elapsed_min = Clock::duration::zero();
    } else {
        elapsed_min = Clock::duration(std::chrono::nanoseconds(1000000000 / v));
    }
	if (elapsed_min_ != elapsed_min) {
        setupFrameTimer();
        elapsed_min_ = elapsed_min;
    }
}

void World::mainLoop(lua_State* L) {
    initSyscalls(L);

	setupFrameTimer();
    auto frame_timer_guard = scopeGuard([this]() {
        uv_timer_stop(&frame_timer_);
		uv_close(reinterpret_cast<uv_handle_t*>(&frame_timer_), nullptr);
	});

    frame_count_ = 0;
    frame_count_per_second_ = 0;
    uv_timer_t frame_count_timer;
    int ret = uv_timer_init(loop(), &frame_count_timer);
    if (ret != 0) {
        throw std::runtime_error("Failed to initialize uv timer for frame count");
    }
    frame_count_timer.data = this;
    uv_timer_start(&frame_count_timer, [](uv_timer_t* p) {
        auto world = reinterpret_cast<World*>(p->data);
        world->frame_count_per_second_ = world->frame_count_;
        world->frame_count_ = 0;
	}, 1000, 1000);
    auto frame_count_timer_guard = scopeGuard([&frame_count_timer]() {
        uv_timer_stop(&frame_count_timer);
        uv_close(reinterpret_cast<uv_handle_t*>(&frame_count_timer), nullptr);
	});

	Vm::Kernel::mainLoop(L);
}

void World::frame(Clock::duration elapsed) {
}

void World::handleEvent(const sf::Event& evt) {
}

void World::setupFrameTimer() {
    uv_timer_stop(&frame_timer_);
    uv_timer_start(&frame_timer_, [](uv_timer_t* p) {
        reinterpret_cast<World*>(p->data)->updateFrame();
    }, 0, std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_min_).count());
    frame_last_ = Clock::now();
    frame_count_ = 0;
    frame_count_per_second_ = 0;
}

void World::processEvents() {
    while (event_queue_start_ != event_queue_end_) {
        const auto& evt = event_queue_[event_queue_start_];
        handleEvent(evt);
        event_queue_start_++;
		event_queue_start_ &= EventQueueMask;
	}
}

void World::updateFrame() {
    auto now = Clock::now();
    auto elapsed = now - frame_last_;

    processEvents();

    if (state_ == State::Ready) {
        if (physics_enabled_) {
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
            edyn::update(scene_, elapsed_ms);
        }
    }

    frame(elapsed);
    frame_last_ = now;
	frame_count_++;
}

void World::initSyscalls(lua_State* L) {
    Vm::GlobalTable api(L, "volcano");

    lua_pushlightuserdata(L, this);
    lua_pushcclosure(L, [](lua_State* T) {
        auto world = reinterpret_cast<World*>(lua_touserdata(T, lua_upvalueindex(1)));
        async::spawn(world->context_, [world] {
            return world->context_.fps();
        }).then(*world, [world, T](unsigned int fps) {
            lua_pushinteger(T, fps);
            world->resumeTask(T);
        });
        return lua_yield(T, 0);
    }, 1);
    lua_setglobal(L, "fps");
}

VOLCANO_FRAMEWORK_END
