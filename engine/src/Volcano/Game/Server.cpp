//
//
#include <thread>

#include <Volcano/Game/Server.hpp>

VOLCANO_GAME_BEGIN

Server::Server(const Napi::CallbackInfo &info):
	Napi::ObjectWrap<Server>(info),
	m_loop(nullptr),
	m_frameCountPerSecond(0),
	m_world(nullptr)
{
}

Server::~Server(void)
{
}

Napi::Function Server::defineClass(Napi::Env env)
{
	return DefineClass(env, "Server", {
		InstanceAccessor<&Server::fps>("fps"),
		InstanceAccessor<&Server::fpsMax, &Server::setFpsMax>("fps"),
		InstanceAccessor<&Server::isStarted>("isStarted"),
		InstanceAccessor<&Server::world, &Server::setWorld>("world"),
		InstanceMethod<&Server::start>("start"),
		InstanceMethod<&Server::stop>("stop")
	});
}

Napi::Value Server::start(const Napi::CallbackInfo &info)
{
	if (m_loop != nullptr)
		return Napi::Value::From(info.Env(), false);

	int port = 7788;

	if (info.Length() > 0) {
		if (info.Length() != 1) {
			// TODO error or exception?
		}
		port = info[0].ToNumber().Int32Value();
	}

	napi_status ns;

	ns = napi_get_uv_event_loop(info.Env(), &m_loop);
	if (ns != napi_ok || m_loop == nullptr)
		return Napi::Value::From(info.Env(), false);

	uv_idle_init(m_loop, &m_frameIdle);
	uv_idle_start(&m_frameIdle, &Server::frameCallback);
	m_frameIdle.data = this;

	uv_udp_init(m_loop, &m_handle);

	sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", port, &addr);
	uv_udp_bind(&m_handle, reinterpret_cast<sockaddr *>(&addr), 0);
	uv_udp_recv_start(&m_handle, &Server::allocCallback, &Server::receiveCallback);
	m_handle.data = this;

	m_frameLast = high_resolution_clock::now();
	m_frameCountLast = m_frameLast;
	m_frameCountPerSecond = 0;
	m_frameCount = 0;

	return Napi::Value::From(info.Env(), true);
}

Napi::Value Server::stop(const Napi::CallbackInfo &info)
{
	if (m_loop == nullptr)
		return Napi::Value();

	uv_idle_stop(&m_frameIdle);
	uv_close_sync(reinterpret_cast<uv_handle_t *>(&m_frameIdle));

	uv_udp_recv_stop(&m_handle);
	uv_close_sync(reinterpret_cast<uv_handle_t *>(&m_handle));

	m_frameCountPerSecond = 0;
	m_loop = nullptr;

	return Napi::Value();
}

Napi::Value Server::isStarted(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_loop != nullptr);
}

Napi::Value Server::fps(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_frameCountPerSecond);
}

Napi::Value Server::fpsMax(const Napi::CallbackInfo &info)
{
	auto usElapsedMin = duration_cast<std::chrono::microseconds>(m_elapsedMin).count();
	return Napi::Value::From(info.Env(), double(usElapsedMin) / 1000000.0);
}

void Server::setFpsMax(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	auto v = value.ToNumber();
	setFpsMax(v.Int32Value());
}

Napi::Value Server::world(const Napi::CallbackInfo &info)
{
	return m_worldValue;
}

void Server::setWorld(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (info.Length() != 1) {
		// ERROR
	}

	if (value.IsNull()) {
		m_world = nullptr;
		m_worldValue = Napi::Value();
		return;
	}

	if (!value.IsObject()) {
		// ERROR
	}

	m_world = World::Unwrap(value.ToObject());
	m_worldValue = value;
}

void Server::setFpsMax(size_t v)
{
	size_t tmp = v;
	if (tmp < 1)
		tmp = 1;

	m_elapsedMin = 1000000000ns / v;
	m_frameLast = high_resolution_clock::now();
	m_frameCount = 0;
	m_frameCountPerSecond = 0;
}

void Server::frameCallback(uv_idle_t *p)
{
	reinterpret_cast<Server *>(p->data)->frame();
}

void Server::frame(void)
{
	auto curr = high_resolution_clock::now();
	auto elapsed = curr - m_frameLast;
	if (elapsed < m_elapsedMin) {
		std::this_thread::yield();
		return;
	}

	if ((curr - m_frameCountLast) >= 1s) {
		m_frameCountLast = curr;
		m_frameCountPerSecond = m_frameCount;
		m_frameCount = 0;
	}

	auto usElapsed = duration_cast<std::chrono::milliseconds>(elapsed).count();
	float floatElapsed = float(usElapsed) / 1000.0f;

	update(floatElapsed);

	m_frameLast = curr;
	m_frameCount += 1;
}

void Server::update(float elapsed)
{
	// handleClients(elapsed);

	if (m_world != nullptr)
		m_world->update(elapsed);

	// dispatchClients(elapsed);
}

void Server::allocCallback(uv_handle_t *handle, size_t suggestedSize, uv_buf_t *buf)
{
}

void Server::receiveCallback(uv_udp_t *req, ssize_t nRead, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags)
{
}

VOLCANO_GAME_END
