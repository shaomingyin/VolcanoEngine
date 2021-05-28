//
//
#ifndef VOLCANO_GAME_CLIENT_HPP
#define VOLCANO_GAME_CLIENT_HPP

#include <string>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Client: public Napi::ObjectWrap<Client> {
	VOLCANO_DISABLE_COPY_AND_MOVE(Client)

public:
	Client(const Napi::CallbackInfo &info);
	virtual ~Client(void);

public:
	static Napi::Function defineClass(Napi::Env env);

private:
	Napi::Value open(const Napi::CallbackInfo &info);
	Napi::Value close(const Napi::CallbackInfo &info);
	Napi::Value isOpen(const Napi::CallbackInfo &info);

private:
	void update(void);
	static void allocCallback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
	static void receiveCallback(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags);

private:
	uv_udp_t m_handle;
	bool m_isOpen;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CLIENT_HPP
