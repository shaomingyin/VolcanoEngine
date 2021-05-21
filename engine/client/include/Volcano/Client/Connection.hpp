//
//
#ifndef VOLCANO_CLIENT_CONNECTION_HPP
#define VOLCANO_CLIENT_CONNECTION_HPP

#include <string>

#include <Volcano/Client/Common.hpp>
#include <Volcano/Client/Window.hpp>

VOLCANO_CLIENT_BEGIN

class Connection: public Napi::ObjectWrap<Connection> {
	VOLCANO_DISABLE_COPY_AND_MOVE(Connection)

public:
	Connection(const Napi::CallbackInfo &info);
	virtual ~Connection(void);

public:
	static void initExports(Napi::Env env, Napi::Object exports);

private:
	Napi::Value open(const Napi::CallbackInfo &info);
	Napi::Value close(const Napi::CallbackInfo &info);
	Napi::Value isOpen(const Napi::CallbackInfo &info);
	Napi::Value window(const Napi::CallbackInfo &info);
	void setWindow(const Napi::CallbackInfo &info, const Napi::Value &value);

private:
	void update(void);
	static void allocCallback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
	static void receiveCallback(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags);

private:
	uv_udp_t m_connection;
	bool m_isOpen;
	Napi::Value m_windowValue;
	Window *m_window;
};

VOLCANO_CLIENT_END

#endif // VOLCANO_CLIENT_CONNECTION_HPP
