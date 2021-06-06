//
//
#include <thread>

#include <Volcano/Game/Client.hpp>

VOLCANO_GAME_BEGIN

Client::Client(const Napi::CallbackInfo &info):
	Node::Object<Client>(info)
{
	uv_loop_t *loop = nullptr;
	napi_status ns = napi_get_uv_event_loop(info.Env(), &loop);
	if (ns != napi_ok || loop == nullptr) {
		Node::throwError(info.Env(), "Failed to get loop object.");
		return;
	}

	if (uv_idle_init(loop, &m_frame) < 0) {
		Node::throwError(info.Env(), "Failed to init frame handle.");
		return;
	}

	m_frame.data = this;

	auto window = Graphics::Window::newInstance(info.Env());
	if (!window.IsObject()) {
		Node::throwError(info.Env(), "Failed to create window object.");
		return;
	}

	m_window = Graphics::Window::Unwrap(window);
	if (m_window == nullptr) {
		Node::throwError(info.Env(), "Failed to unwrap window object.");
		return;
	}

	m_windowRef = Napi::Persistent(window);
	m_windowRef.SuppressDestruct();

	setFpsMax(60);
}

Client::~Client(void)
{
	uv_close(reinterpret_cast<uv_handle_t *>(&m_connection), nullptr);
}

void Client::registerClass(Napi::Env env)
{
	registerConstructor(env, "Frame", {
		InstanceMethod<&Client::start>("start"),
		InstanceMethod<&Client::stop>("stop"),
		InstanceAccessor<&Client::started>("started"),
		InstanceAccessor<&Client::fps>("fps"),
		InstanceAccessor<&Client::fpsMax, &Client::setFpsMax>("fpsMax")
	});
}

Napi::Value Client::start(const Napi::CallbackInfo &info)
{
	uv_idle_start(&m_frame, [](uv_idle_t *p) {
		reinterpret_cast<Client *>(p->data)->update();
	});

	m_frameLast = Clock::now();
	m_frameCountLast = m_frameLast;
	m_frameCount = 0;
	m_frameCountPerSecond = 0;

	return Napi::Value::From(info.Env(), true);
}

Napi::Value Client::stop(const Napi::CallbackInfo &info)
{
	uv_idle_stop(&m_frame);

	return Napi::Value();
}

Napi::Value Client::started(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(),
		uv_is_active(reinterpret_cast<uv_handle_t *>(&m_frame)));
}

Napi::Value Client::fps(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_frameCountPerSecond);
}

Napi::Value Client::fpsMax(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), 1000000000ns / m_elapsedMin);
}

void Client::setFpsMax(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	setFpsMax(value.ToNumber().Int32Value());
}

void Client::setFpsMax(int v)
{
	int tmp = v;
	if (tmp < 1)
		tmp = 1;

	m_elapsedMin = 1000000000ns / tmp;

	m_frameLast = Clock::now();
	m_frameCountLast = m_frameLast;
	m_frameCount = 0;
	m_frameCountPerSecond = 0;
}

void Client::update(void)
{
	auto curr = Clock::now();

	if ((curr - m_frameCountLast) >= 1s) {
		m_frameCountLast = curr;
		m_frameCountPerSecond = m_frameCount;
		m_frameCount = 0;
	}

	auto elapsed = curr - m_frameLast;
	if (elapsed >= m_elapsedMin) {
		frame(elapsed);
		m_frameLast = curr;
		m_frameCount += 1;
	} else
		std::this_thread::yield();
}

void Client::frame(Duration elapsed)
{
}

VOLCANO_GAME_END
