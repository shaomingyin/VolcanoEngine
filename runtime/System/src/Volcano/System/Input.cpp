//
//
#include <Volcano/System/Input.h>

VOLCANO_SYSTEM_BEGIN

static uint32_t makeId(SDL_KeyCode key_code, Uint16 mod) {
	return (static_cast<uint32_t>(key_code) & 0xFFFFU) | ((static_cast<uint32_t>(mod) << 16) & 0xFFFF0000U);
}

static SDL_KeyCode keyCodeFromId(uint32_t id) {
	return static_cast<SDL_KeyCode>(id & 0xFFFFU);
}

static Uint16 keyModifierFromId(uint32_t id) {
	return static_cast<Uint16>((id >> 16) & 0xFFFFU);
}

Input::Input(Uint32 window_id)
	: window_id_(window_id)
	, mouse_sensitivity_(1.0f) {
}

void Input::bindKey(SDL_KeyCode key_code, KeyAction key_action, KeyState key_state, Uint16 modifier) {
	std::lock_guard<std::mutex> lg(mutex_);
	//key_bindings_[makeId(key_code, modifier)] = std::move(key_action);
}

void Input::handleEvent(const SDL_Event& evt) {
	switch (evt.type) {
	case SDL_MOUSEMOTION:
		handleMouseMovement(evt);
		break;
	case SDL_MOUSEBUTTONDOWN:
		handleMouseButtonDown(evt);
		break;
	case SDL_MOUSEBUTTONUP:
		handleMouseButtonUp(evt);
		break;
	case SDL_KEYDOWN:
		handleKeyDown(evt);
		break;
	case SDL_KEYUP:
		handleKeyUp(evt);
		break;
	}
}

void Input::handleMouseMovement(const SDL_Event& evt) {
}

void Input::handleMouseButtonDown(const SDL_Event& evt) {
}

void Input::handleMouseButtonUp(const SDL_Event& evt) {
}

void Input::handleKeyDown(const SDL_Event& evt) {
}

void Input::handleKeyUp(const SDL_Event& evt) {
}

VOLCANO_SYSTEM_END
