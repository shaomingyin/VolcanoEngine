//
//
#ifndef VOLCANO_GAME_SERVER_HPP
#define VOLCANO_GAME_SERVER_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

class Server: public Napi::ObjectWrap<Server> {
	VOLCANO_DISABLE_COPY_AND_MOVE(Server)

public:
	Server(const Napi::CallbackInfo &info);
	virtual ~Server(void);

public:
	static Napi::Function defineClass(Napi::Env env);

private:
	Napi::Value start(const Napi::CallbackInfo &info);
	Napi::Value stop(const Napi::CallbackInfo &info);
	Napi::Value isStarted(const Napi::CallbackInfo &info);
	Napi::Value fps(const Napi::CallbackInfo &info);
	Napi::Value fpsMax(const Napi::CallbackInfo &info);
	void setFpsMax(const Napi::CallbackInfo &info, const Napi::Value &value);
	Napi::Value world(const Napi::CallbackInfo &info);
	void setWorld(const Napi::CallbackInfo &info, const Napi::Value &value);

private:
	void setFpsMax(size_t v);
	static void frameCallback(uv_idle_t *p);
	void frame(void);
	void update(float elapsed);
	static void allocCallback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
	static void receiveCallback(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags);

private:
	uv_loop_t *m_loop;
	int m_loopRef;
	uv_udp_t m_handle;
	uv_idle_t m_frameIdle;
	high_resolution_clock::time_point m_frameLast;
	high_resolution_clock::time_point m_frameCountLast;
	high_resolution_clock::duration m_elapsedMin;
	size_t m_frameCount;
	size_t m_frameCountPerSecond;
	Napi::Value m_worldValue;
	World *m_world;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SERVER_HPP
