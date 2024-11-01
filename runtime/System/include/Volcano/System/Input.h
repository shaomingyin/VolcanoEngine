//
//
#ifndef VOLCANO_SYSTEM_INPUT_H
#define VOLCANO_SYSTEM_INPUT_H

#include <mutex>
#include <functional>
#include <unordered_map>

#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Input {
public:
	enum class KeyState {
		Down = 0,
		Up,
		Pressed
	};

	using KeyAction = std::function<void()>;
	using KeyBindings = std::unordered_map<uint32_t, KeyAction>;

public:
	Input(Uint32 window_id);
	virtual ~Input() = default;

public:
	float mouseSensitivity() const {
		return mouse_sensitivity_;
	}

	void setMouseSensitivity(float v) {
		mouse_sensitivity_ = std::clamp(v, 0.001f, 100.0f);
	}

	void clearAllKeyBindings() {
		std::lock_guard<std::mutex> lg(mutex_);
		key_bindings_.clear();
	}

	const KeyBindings& keyBindings() const {
		return key_bindings_;
	}

	void bindKey(SDL_KeyCode key_code, KeyAction key_action, KeyState key_state = KeyState::Down, Uint16 modifier = 0);
	void handleEvent(const SDL_Event& evt);

protected:
	void handleMouseMovement(const SDL_Event& evt);
	void handleMouseButtonDown(const SDL_Event& evt);
	void handleMouseButtonUp(const SDL_Event& evt);
	void handleKeyDown(const SDL_Event& evt);
	void handleKeyUp(const SDL_Event& evt);

private:
	struct KeyBinding {
		KeyAction action;
		bool pressed;
	};

	mutable std::mutex mutex_;
	Uint32 window_id_;
	KeyBindings key_bindings_;
	float mouse_sensitivity_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_INPUT_H
