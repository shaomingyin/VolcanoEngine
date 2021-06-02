//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Game/MultiPlayer.hpp>

VOLCANO_GAME_BEGIN

MultiPlayer::MultiPlayer(const Napi::CallbackInfo &info):
	Frame(info)
{
	if (uv_udp_init(loop(), &m_handle) < 0) {
		Node::throwError(info.Env(), "Failed to init udp handle.");
		return;
	}
}

MultiPlayer::~MultiPlayer(void)
{
	uv_close_sync(reinterpret_cast<uv_handle_t *>(&m_handle));
}

Napi::Function MultiPlayer::constructor(Napi::Env env)
{
	return Node::defineClass<MultiPlayer, Frame>(env, "MultiPlayer", {
		InstanceAccessor<&MultiPlayer::host, &MultiPlayer::setHost>("host"),
		InstanceAccessor<&MultiPlayer::port, &MultiPlayer::setPort>("port")
	});
}

bool MultiPlayer::init(void)
{
	if (!Frame::init())
		return false;

	sockaddr_in addr;
	uv_ip4_addr(m_host.c_str(), m_port, &addr);
	uv_udp_connect(&m_handle, (sockaddr *)&addr);
	uv_udp_recv_start(&m_handle, &MultiPlayer::allocCallback, &MultiPlayer::receiveCallback);
	m_handle.data = this;

	// TODO attach to server.

	return true;
}

void MultiPlayer::shutdown(void)
{
	uv_udp_recv_stop(&m_handle);
	uv_udp_connect(&m_handle, nullptr);

	Frame::shutdown();
}

void MultiPlayer::update(Duration elapsed)
{
}

Napi::Value MultiPlayer::host(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_host);
}

void MultiPlayer::setHost(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	std::string v = value.ToString();
	if (v == m_host)
		return;

	if (isStarted()) {
		Node::throwError(info.Env(), "Cannot set host while started.");
		return;
	}

	m_host = v;
}

Napi::Value MultiPlayer::port(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_port);
}

void MultiPlayer::setPort(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	int port = value.ToNumber().Int32Value();
	if (port == m_port)
		return;

	if (isStarted()) {
		Node::throwError(info.Env(), "Cannot set port while started.");
		return;
	}

	m_port = port;
}

void MultiPlayer::allocCallback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
}

void MultiPlayer::receiveCallback(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags)
{
}

VOLCANO_GAME_END
