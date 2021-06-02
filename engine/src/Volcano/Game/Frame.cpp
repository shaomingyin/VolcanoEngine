//
//
#include <thread>

#include <Volcano/Game/Frame.hpp>

VOLCANO_GAME_BEGIN

Frame::Frame(const Napi::CallbackInfo &info):
	Napi::ObjectWrap<Frame>(info),
	m_frameCount(0),
	m_frameCountPerSecond(0)
{
	uv_loop_t *loop = nullptr;
	napi_status ns = napi_get_uv_event_loop(info.Env(), &loop);
	if (ns != napi_ok || loop == nullptr) {
		Node::throwError(info.Env(), "Failed to get loop object.");
		return;
	}

	if (uv_idle_init(loop, &m_handle) < 0) {
		Node::throwError(info.Env(), "Failed to init frame handle.");
		return;
	}
}

Frame::~Frame(void)
{
	uv_close(reinterpret_cast<uv_handle_t *>(&m_handle), nullptr);
}

Napi::Function Frame::constructor(Napi::Env env)
{
	return Node::defineClass<Frame>(env, "Frame", {
		InstanceMethod<&Frame::start>("start"),
		InstanceMethod<&Frame::stop>("stop"),
		InstanceAccessor<&Frame::started>("started")
	});
}

uv_loop_t *Frame::loop(void)
{
	return m_handle.loop;
}

bool Frame::isStarted(void) const
{
	return uv_is_active(reinterpret_cast<const uv_handle_t *>(&m_handle));
}

bool Frame::init(void)
{
	uv_idle_start(&m_handle, [](uv_idle_t *p) {
		auto frame = reinterpret_cast<Frame *>(p->data);
		auto curr = Clock::now();
		auto elapsed = curr - frame->m_frameLast;
		if (elapsed >= frame->m_elapsedMin) {
			frame->update(elapsed);
			frame->m_frameLast = curr;
			frame->m_frameCount += 1;
			if ((curr - frame->m_frameCountLast) >= 1s) {
				frame->m_frameCountLast = curr;
				frame->m_frameCountPerSecond = frame->m_frameCount;
				frame->m_frameCount = 0;
			}
		} else
			std::this_thread::yield();
	});

	m_handle.data = this;

	m_frameLast = Clock::now();
	m_frameCountLast = m_frameLast;
	m_frameCount = 0;
	m_frameCountPerSecond = 0;

	return true;
}

void Frame::shutdown(void)
{
	uv_idle_stop(&m_handle);
}

void Frame::update(Duration elapsed)
{
}

Napi::Value Frame::start(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), init());
}

Napi::Value Frame::stop(const Napi::CallbackInfo &info)
{
	shutdown();
	return Napi::Value();
}

Napi::Value Frame::started(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), isStarted());
}

Napi::Value Frame::fps(const Napi::CallbackInfo &info) const
{
	return Napi::Value::From(info.Env(), m_frameCountPerSecond);
}

Napi::Value Frame::fpsMax(const Napi::CallbackInfo &info) const
{
	return Napi::Value::From(info.Env(), 1000000000ns / m_elapsedMin);
}

void Frame::setFpsMax(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	int tmp = value.ToNumber().Int32Value();
	if (tmp < 1)
		tmp = 1;

	m_elapsedMin = 1000000000ns / tmp;

	m_frameLast = Clock::now();
	m_frameCountLast = m_frameLast;
	m_frameCount = 0;
	m_frameCountPerSecond = 0;
}

VOLCANO_GAME_END
