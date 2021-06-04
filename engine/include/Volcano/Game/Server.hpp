//
//
#ifndef VOLCANO_GAME_SERVER_HPP
#define VOLCANO_GAME_SERVER_HPP

#include <Volcano/Node.hpp>
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

class Server: public Node::Object<Server> {
public:
	Server(const Napi::CallbackInfo &info);
	virtual ~Server(void);

public:
	static Napi::Function defineConstructor(Napi::Env env);

private:
	Napi::Value start(const Napi::CallbackInfo &info);
	Napi::Value stop(const Napi::CallbackInfo &info);
	Napi::Value isStarted(const Napi::CallbackInfo &info);
	Napi::Value world(const Napi::CallbackInfo &info);

private:
	void update(void);
	void frame(Duration elapsed);
	static void allocCallback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
	static void receiveCallback(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags);

private:
	int m_port;
	uv_udp_t m_handle;
	Napi::ObjectReference m_worldRef;
	World *m_world;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SERVER_HPP
