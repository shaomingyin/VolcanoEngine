//
//
#ifndef VOLCANO_SOCKET_HPP
#define VOLCANO_SOCKET_HPP

#include <functional>
#include <string_view>

#include <flatbuffers/flexbuffers.h>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Socket {
	VOLCANO_DISABLE_COPY_AND_MOVE(Socket)

public:
	enum class State {
		Idle = 0,
		Listening,
		Connected,
		Disconnected,
		Error
	};

	enum class Result: uint32_t {
		Ok = 0,
		Unknown
	};

	using SendCallback = uv_udp_send_cb;

	static constexpr int DefaultPort = 7788;

public:
	Socket(uv_loop_t *loop);
	virtual ~Socket(void);

public:
	State state(void) const;
	bool bind(int port = DefaultPort);
	bool bind(std::string_view ip, int port = DefaultPort);
	bool connect(std::string_view ip, int port = DefaultPort);
	void disconnect(void);

public:
	bool attach(std::function<Result> cb);
	bool detach(std::function<Result> cb);
	
public:
	sigslot::signal<State> onStateChanged;
	sigslot::signal<std::string_view, int> onAttach;
	sigslot::signal<std::string_view, int> onDetach;

private:
	void setState(State v);
	static void allocCallback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
	static void receiveCallback(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags);

private:
	enum class Request : uint32_t {
		Nop = 0,
		Attach,
		Detach
	};

private:
	uv_udp_t m_handle;
	State m_state;
	flexbuffers::Builder m_txBuffer;
};

VOLCANO_END

#endif // VOLCANO_SOCKET_HPP
