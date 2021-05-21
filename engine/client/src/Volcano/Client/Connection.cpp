//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Client/Connection.hpp>

VOLCANO_CLIENT_BEGIN

Connection::Connection(const Napi::CallbackInfo &info):
	Napi::ObjectWrap<Connection>(info),
	m_window(nullptr)
{
}

Connection::~Connection(void)
{
}

void Connection::initExports(Napi::Env env, Napi::Object exports)
{
	Napi::Function constructor = DefineClass(env, "Connection", {
		InstanceMethod<&Connection::open>("open"),
		InstanceMethod<&Connection::close>("close"),
		InstanceMethod<&Connection::isOpen>("isOpen")
	});

	exports.Set("Connection", constructor);
}

Napi::Value Connection::open(const Napi::CallbackInfo &info)
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

	uv_udp_init(loop, &m_connection);

	sockaddr_in addr;
	uv_ip4_addr(ip.c_str(), port, &addr);
	uv_udp_connect(&m_connection, (sockaddr *)&addr);
	uv_udp_recv_start(&m_connection, &Connection::allocCallback, &Connection::receiveCallback);
	m_connection.data = this;

	// TODO attach to server.

	m_isOpen = true;

	return Napi::Value::From(info.Env(), true);
}

Napi::Value Connection::close(const Napi::CallbackInfo &info)
{
	if (!m_isOpen)
		return Napi::Error::New(info.Env(), "Not open.").Value();

	uv_udp_recv_stop(&m_connection);
	uv_udp_connect(&m_connection, nullptr);
	m_isOpen = false;

	return Napi::Value();
}

Napi::Value Connection::isOpen(const Napi::CallbackInfo &info)
{
	return Napi::Value::From(info.Env(), m_isOpen);
}

Napi::Value Connection::window(const Napi::CallbackInfo &info)
{
	return m_windowValue;
}

void Connection::setWindow(const Napi::CallbackInfo &info, const Napi::Value &value)
{
	if (info.Length() != 1) {
		// ERROR
	}

	if (value.IsNull()) {
		m_window = nullptr;
		m_windowValue = Napi::Value();
		return;
	}

	if (!value.IsObject()) {
		// ERROR
	}

	m_window = Window::Unwrap(value.ToObject());
	m_windowValue = value;
}

void Connection::update(void)
{
}

void Connection::allocCallback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{

}

void Connection::receiveCallback(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags)
{

}

VOLCANO_CLIENT_END
