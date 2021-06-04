//
//
#ifndef VOLCANO_GAME_CLIENT_HPP
#define VOLCANO_GAME_CLIENT_HPP

#include <Volcano/Node.hpp>
#include <Volcano/Graphics/Window.hpp>
#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Client: public Node::Object<Client> {
public:
    Client(const Napi::CallbackInfo &infO);
    ~Client(void) override;

public:
    static Napi::Function defineConstructor(Napi::Env env);

private:
    Napi::Value start(const Napi::CallbackInfo &info);
	Napi::Value stop(const Napi::CallbackInfo &info);
	Napi::Value started(const Napi::CallbackInfo &info);
	Napi::Value fps(const Napi::CallbackInfo &info);
	Napi::Value fpsMax(const Napi::CallbackInfo &info);
	void setFpsMax(const Napi::CallbackInfo &info, const Napi::Value &value);

private:
    void setFpsMax(int v);
    void update(void);
    void frame(Duration elapsed);

private:
    uv_udp_t m_connection;
    uv_idle_t m_frame;
	Duration m_elapsedMin;
	TimePoint m_frameLast;
	TimePoint m_frameCountLast;
	int m_frameCount;
	int m_frameCountPerSecond;
    Napi::ObjectReference m_windowRef;
    Graphics::Window *m_window;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CLIENT_HPP
