//
//
#include <Volcano/Socket.hpp>

VOLCANO_BEGIN

Socket::Socket(uv_loop_t *loop):
	m_state(State::Idle)
{
	uv_udp_init(loop, &m_handle);
}

Socket::~Socket(void)
{
	// TODO close handle
}

Socket::State Socket::state(void) const
{
	return m_state;
}

bool Socket::bind(int port)
{
	return bind("0.0.0.0");
}

bool Socket::bind(std::string_view ip, int port)
{
	VOLCANO_ASSERT(m_state == State::Idle);

	int ret;
	sockaddr_in addr;
	uv_ip4_addr(ip.data(), port, &addr);

	ret = uv_udp_bind(&m_handle, reinterpret_cast<sockaddr *>(&addr), 0);
	if (ret < 0)
		return false;

	uv_udp_recv_start(&m_handle, &Socket::allocCallback, &Socket::receiveCallback);
	setState(State::Listening);

	return true;
}

bool Socket::connect(std::string_view ip, int port)
{
	VOLCANO_ASSERT(m_state == State::Idle);
	int ret;
	sockaddr_in addr;
	uv_ip4_addr(ip.data(), port, &addr);

	ret = uv_udp_connect(&m_handle, reinterpret_cast<sockaddr *>(&addr));
	if (ret < 0)
		return false;

	uv_udp_recv_start(&m_handle, &Socket::allocCallback, &Socket::receiveCallback);
	setState(State::Connected);

	return true;
}

void Socket::disconnect(void)
{
	VOLCANO_ASSERT(m_state == State::Connected);

	uv_udp_recv_stop(&m_handle);
	uv_udp_connect(&m_handle, nullptr);
	setState(State::Idle);
}

size_t Socket::recv(ByteArray &out)
{

}
size_t Socket::recv(void *buf, size_t len);
size_t Socket::send(const ByteArray &data);
size_t Socket::send(const void *buf, size_t len);

void Socket::setState(State v)
{
	if (m_state != v) {
		m_state = v;
		onStateChanged(v);
	}
}

void Socket::allocCallback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
void Socket::receiveCallback(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags);

VOLCANO_END
