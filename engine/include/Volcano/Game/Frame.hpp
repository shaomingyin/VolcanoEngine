//
//
#ifndef VOLCANO_GAME_FRAME_HPP
#define VOLCANO_GAME_FRAME_HPP

#include <Volcano/Node.hpp>
#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

template <typename T>
class Frame: public Napi::ObjectWrap<T> {
public:
	Frame(const Napi::CallbackInfo &info);
	~Frame(void) override;

public:
	static Napi::Function constructor(Napi::Env env);
	uv_loop_t *loop(void);
	bool isStarted(void) const;

protected:
	virtual bool init(void);
	virtual void shutdown(void);
	virtual void update(Duration elapsed);

private:
	Napi::Value start(const Napi::CallbackInfo &info);
	Napi::Value stop(const Napi::CallbackInfo &info);
	Napi::Value started(const Napi::CallbackInfo &info);
	Napi::Value fps(const Napi::CallbackInfo &info) const;
	Napi::Value fpsMax(const Napi::CallbackInfo &info) const;
	void setFpsMax(const Napi::CallbackInfo &info, const Napi::Value &value);

private:
	uv_idle_t m_handle;
	Duration m_elapsedMin;
	TimePoint m_frameLast;
	TimePoint m_frameCountLast;
	int m_frameCount;
	int m_frameCountPerSecond;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_FRAME_HPP
