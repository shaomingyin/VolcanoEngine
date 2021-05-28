//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Game/Client.hpp>

VOLCANO_GAME_BEGIN

Client::Client(const Napi::CallbackInfo &info) :
	Napi::ObjectWrap<Client>(info)
{
}

Client::~Client(void)
{
}

Napi::Function Client::defineClass(Napi::Env env)
{
	return DefineClass(env, "Client", {
		InstanceMethod<&Client::open>("open"),
		InstanceMethod<&Client::close>("close"),
		InstanceMethod<&Client::isOpen>("isOpen")
	});
}

Napi::Value Client::open(const Napi::CallbackInfo &info)
{
	std::string ip;
	int port = 7788;

	switch (info.Length()) {
	case 2:
		port = info[1].ToNumber().Int32Value();
	case 1:
		ip = info[0].ToString();
		break;
	default:
		return Napi::Error::New(info.Env(), "Invalid parameter(s).").Value();
		break;
	}

	napi_status ns;
	uv_loop_t *loop = nullptr;

	ns = napi_get_uv_event_loop(info.Env(), &loop);
	if (ns != napi_ok || loop == nullptr)
		return Napi::Error::New(info.Env(), "Failed to get uv loop.").Value();

	uv_udp_init(loop, &m_handle);

	sockaddr_in addr;
	uv_ip4_addr(ip.c_str(), port, &addr);
	uv_udp_connect(&m_handle, (sockaddr *)&addr);
	uv_udp_recv_start(&m_handle, &Client::allocCallback, &Client::receiveCallback);
	m_handle.data = this;

	// TODO attach to server.

	m_isOpen = true;

	return Napi::Value::From(info.Env(), true);
}

Napi::Value Client::close(const Napi::CallbackInfo &info)
{
	if (!m_isOpen)
		return Napi::Error::New(info.Env(), "Not open.").Value();

	uv_udp_recv_stop(&m_handle);
	uv_udp_connect(&m_handle, nullptr);
	m_isOpen = false;

	return Napi::Value();
}

Napi::Value Client::isOpen(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_isOpen);
}

void Client::update(void)
{
}

void Client::allocCallback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{

}

void Client::receiveCallback(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags)
{

}

VOLCANO_GAME_END
