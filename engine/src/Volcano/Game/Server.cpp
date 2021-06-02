//
//
#include <thread>

#include <Volcano/Game/Server.hpp>

VOLCANO_GAME_BEGIN

Server::Server(const Napi::CallbackInfo &info):
	Napi::ObjectWrap<Server>(info),
	m_port(7788),
	m_world(nullptr)
{
	uv_loop_t *loop = nullptr;
	napi_status ns = napi_get_uv_event_loop(info.Env(), &loop);
	if (ns != napi_ok) {
		Napi::ThrowError(info.Env(), "Cannot get loop.");
		return;
	}

	if (!baseInit(loop)) {
		Napi::ThrowError(info.Env(), "Failed to init frame object.");
		return;
	}

	if (uv_udp_init(loop, &m_handle) < 0) {
		Napi::ThrowError(info.Env(), "Failed to init udp handle.");
		return;
	}

	auto worldObject = World::newInstance({});
	if (!worldObject.IsObject()) {
		Napi::ThrowError(info.Env(), "Cannot create world object.");
		return;
	}

	m_world = World::Unwrap(worldObject);
	if (m_world == nullptr) {
		Napi::ThrowError(info.Env(), "Cannot get world object.");
		return;
	}

	m_worldRef = Napi::Persistent(worldObject);
}

Server::~Server(void)
{
	uv_close_sync(reinterpret_cast<uv_handle_t *>(&m_handle));
}

bool Server::start(void)
{
	if (!Frame::start())
		return false;

	sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", m_port, &addr);
	uv_udp_bind(&m_handle, reinterpret_cast<sockaddr *>(&addr), 0);
	uv_udp_recv_start(&m_handle, &Server::allocCallback, &Server::receiveCallback);
	m_handle.data = this;

	return true;
}

void Server::stop(void)
{
	uv_udp_recv_stop(&m_handle);

	Frame::stop();
}

Napi::Function Server::constructor(Napi::Env env)
{
	return Node::defineClass<Server, Frame>(env, "Server", {
		InstanceMethod<&Server::start>("start"),
		InstanceMethod<&Server::stop>("stop"),
		InstanceAccessor<&Server::port, &Server::setPort>("port"),
		InstanceAccessor<&Server::isStarted>("isStarted")
	});
}

void Server::update(Duration elapsed)
{
}

Napi::Value Server::port(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_port);
}

void Server::setPort(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	int port = value.ToNumber().Int32Value();
	if (port == m_port)
		return;

	m_port = port;

	if (Frame::isStarted())
		restart();
}

Napi::Value Server::start(const Napi::CallbackInfo &info)
{
	if (!Frame::start())
		return Napi::Value::From(info.Env(), false);

	if (!start()) {
		Frame::stop();
		return Napi::Value::From(info.Env(), false);
	}

	return Napi::Value::From(info.Env(), true);
}

Napi::Value Server::stop(const Napi::CallbackInfo &info)
{
	stop();
	return Napi::Value();
}

Napi::Value Server::isStarted(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(),
		uv_is_active(reinterpret_cast<uv_handle_t *>(&m_handle)));
}

Napi::Value Server::world(const Napi::CallbackInfo &info)
{
	return m_worldRef.Value();
}

void Server::allocCallback(uv_handle_t *handle, size_t suggestedSize, uv_buf_t *buf)
{
}

void Server::receiveCallback(uv_udp_t *req, ssize_t nRead, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags)
{
}

VOLCANO_GAME_END
