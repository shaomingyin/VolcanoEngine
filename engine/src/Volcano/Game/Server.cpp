//
//
#include <thread>

#include <Volcano/Game/Server.hpp>

VOLCANO_GAME_BEGIN

Server::Server(const Napi::CallbackInfo &info):
	Node::Object<Server>(info),
	m_world(nullptr)
{
	uv_loop_t *loop = nullptr;
	napi_status ns = napi_get_uv_event_loop(info.Env(), &loop);
	if (ns != napi_ok) {
		Node::throwError(info.Env(), "Cannot get loop.");
		return;
	}

	if (uv_udp_init(loop, &m_handle) < 0) {
		Node::throwError(info.Env(), "Failed to init udp handle.");
		return;
	}

	auto worldObject = World::newInstance(info.Env());
	if (!worldObject.IsObject()) {
		Node::throwError(info.Env(), "Cannot create world object.");
		return;
	}

	m_world = World::Unwrap(worldObject);
	if (m_world == nullptr) {
		Node::throwError(info.Env(), "Cannot get world object.");
		return;
	}

	m_worldRef = Napi::Persistent(worldObject);
	m_worldRef.SuppressDestruct();
}

Server::~Server(void)
{
	uv_close_sync(reinterpret_cast<uv_handle_t *>(&m_handle));
}

Napi::Function Server::defineConstructor(Napi::Env env)
{
	return defineClass(env, "Server", {
		InstanceMethod<&Server::start>("start"),
		InstanceMethod<&Server::stop>("stop"),
		InstanceAccessor<&Server::isStarted>("isStarted")
	});
}

Napi::Value Server::start(const Napi::CallbackInfo &info)
{
	// TODO args

	sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", 7788, &addr);
	uv_udp_bind(&m_handle, reinterpret_cast<sockaddr *>(&addr), 0);
	uv_udp_recv_start(&m_handle, &Server::allocCallback, &Server::receiveCallback);
	m_handle.data = this;

	return Napi::Value::From(info.Env(), true);
}

Napi::Value Server::stop(const Napi::CallbackInfo &info)
{
	uv_udp_recv_stop(&m_handle);

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

void Server::update(void)
{
}

void Server::frame(Duration elapsed)
{

}

void Server::allocCallback(uv_handle_t *handle, size_t suggestedSize, uv_buf_t *buf)
{
}

void Server::receiveCallback(uv_udp_t *req, ssize_t nRead, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags)
{
}

VOLCANO_GAME_END
