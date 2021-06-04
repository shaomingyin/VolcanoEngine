//
//
#include <Volcano/Graphics/Window.hpp>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Game/Level.hpp>
#include <Volcano/Game/Server.hpp>
#include <Volcano/Game/Client.hpp>

#include <Volcano/Module/Common.hpp>

VOLCANO_MODULE_BEGIN

static thread_local uv_timer_t pollEventsTimer;
static uint64_t pollEventsIntervalValue = 20;

static Napi::Value version(const Napi::CallbackInfo &info)
{
	auto value = Napi::Array::New(info.Env());

	value[0u] = VOLCANO_VERSION_MAJOR;
	value[1u] = VOLCANO_VERSION_MINOR;
	value[2u] = VOLCANO_VERSION_PATCH;

	return value;
}

static Napi::Value pollEventsInterval(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), pollEventsIntervalValue);
}

static void setPollEventsInternal(const Napi::CallbackInfo &info)
{
	if (info.Length() != 1)
		Node::throwError(info.Env(), "Invalid parameter.");

	pollEventsIntervalValue = info[0].ToNumber().Int64Value();
	if (pollEventsIntervalValue <= 0)
		pollEventsIntervalValue = 1;

	if (uv_is_active(reinterpret_cast<uv_handle_t *>(&pollEventsTimer)))
		uv_timer_set_repeat(&pollEventsTimer, pollEventsIntervalValue);
}

static void start(const Napi::CallbackInfo &info)
{
	if (!uv_is_active(reinterpret_cast<uv_handle_t *>(&pollEventsTimer))) {
		uv_timer_start(&pollEventsTimer,
			[](uv_timer_t *) { glfwPollEvents(); }, 0, pollEventsIntervalValue);
	}
}

static void stop(const Napi::CallbackInfo &info)
{
	if (uv_is_active(reinterpret_cast<uv_handle_t *>(&pollEventsTimer)))
		uv_timer_stop(&pollEventsTimer);
}

static napi_value init(napi_env env, napi_value exports)
{
	auto logger = spdlog::default_logger();

#ifdef VOLCANO_DEBUG
	logger->set_level(spdlog::level::debug);
#else
	logger->set_level(spdlog::level::warning);
#endif

	spdlog::info("asdfasdf");

	if (glfwInit() != GLFW_TRUE)
		return Napi::Error::New(env, "Failed to initialize GLFW.").Value();

	napi_set_instance_data(env, nullptr, [](napi_env, void *, void *) {
		uv_close_sync(&pollEventsTimer);
		glfwTerminate();
	}, nullptr);

	napi_status ns;
	uv_loop_t *loop = nullptr;

	ns = napi_get_uv_event_loop(env, &loop);
	if (ns != napi_ok || loop == nullptr)
		return Napi::Error::New(env, "Failed to get uv loop.").Value();

	uv_timer_init(loop, &pollEventsTimer);

	auto root = Napi::Object(env, exports);

	root.DefineProperty(Napi::PropertyDescriptor::Accessor<version>("version"));
	root.DefineProperty(Napi::PropertyDescriptor::Accessor<pollEventsInterval, setPollEventsInternal>("pollEventsInterval"));

	root["start"] = Napi::Function::New(env, start);
	root["stop"] = Napi::Function::New(env, stop);

	auto graphics = Napi::Object::New(env);
	Graphics::Renderer::defineConstructor(env);
	graphics["Window"] = Graphics::Window::defineConstructor(env);
	root["graphics"] = graphics;

	auto game = Napi::Object::New(env);
	Game::World::defineConstructor(env);
	Game::Level::defineConstructor(env);
	game["Server"] = Game::Server::defineConstructor(env);
	game["Client"] = Game::Client::defineConstructor(env);
	root["game"] = game;

	return exports;
}

VOLCANO_MODULE_END

NAPI_MODULE(NODE_GYP_MODULE_NAME, Volcano::Module::init)
