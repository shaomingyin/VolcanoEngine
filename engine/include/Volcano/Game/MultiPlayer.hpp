//
//
#ifndef VOLCANO_GAME_MULTIPLAYER_HPP
#define VOLCANO_GAME_MULTIPLAYER_HPP

#include <string>

#include <Volcano/Node.hpp>
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Frame.hpp>

VOLCANO_GAME_BEGIN

class MultiPlayer: public Frame {
	VOLCANO_DISABLE_COPY_AND_MOVE(MultiPlayer)

public:
	MultiPlayer(const Napi::CallbackInfo &info);
	virtual ~MultiPlayer(void);

public:
	static Napi::Function constructor(Napi::Env env);

protected:
	bool init(void) override;
	void shutdown(void) override;
	void update(Duration elapsed) override;

private:
	Napi::Value host(const Napi::CallbackInfo &info);
	void setHost(const Napi::CallbackInfo &info, const Napi::Value &value);
	Napi::Value port(const Napi::CallbackInfo &info);
	void setPort(const Napi::CallbackInfo &info, const Napi::Value &value);

private:
	static void allocCallback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
	static void receiveCallback(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags);

private:
	std::string m_host;
	int m_port;
	uv_udp_t m_handle;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MULTIPLAYER_HPP
