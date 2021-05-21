//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Client/Common.hpp>
#include <Volcano/Client/Window.hpp>
#include <Volcano/Client/Connection.hpp>

VOLCANO_CLIENT_BEGIN

static thread_local uv_timer_t pollEventsTimer;

static napi_value init(napi_env env, napi_value exports)
{
	if (glfwInit() != GLFW_TRUE)
		return Napi::Error::New(env, "Failed to initialize GLFW.").Value();

	ScopeGuard glfwGuard([] { glfwTerminate(); });

	napi_status ns;
	uv_loop_t *loop = nullptr;

	ns = napi_get_uv_event_loop(env, &loop);
	if (ns != napi_ok || loop == nullptr)
		return Napi::Error::New(env, "Failed to get uv loop.").Value();

	uv_timer_init(loop, &pollEventsTimer);
	uv_timer_start(&pollEventsTimer, [](uv_timer_t *) { glfwPollEvents(); }, 0, 20);

	napi_set_instance_data(env, nullptr, [](napi_env, void *, void *) {
		uv_timer_stop(&pollEventsTimer);
		uv_close(reinterpret_cast<uv_handle_t *>(&pollEventsTimer), nullptr);
		glfwTerminate();
	}, nullptr);

	Window::initExports(env, Napi::Object(env, exports));
	Connection::initExports(env, Napi::Object(env, exports));

	glfwGuard.dismiss();

	return exports;
}

VOLCANO_CLIENT_END

NAPI_MODULE(NODE_GYP_MODULE_NAME, Volcano::Client::init)
