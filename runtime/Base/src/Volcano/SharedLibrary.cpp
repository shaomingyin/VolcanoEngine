//
//
#include <Volcano/SharedLibrary.h>

VOLCANO_BEGIN

SharedLibrary::SharedLibrary(const char* path)
	: handle_(SDL_LoadObject(path)) {
	if (handle_ == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}
}

SharedLibrary::~SharedLibrary() {
	SDL_UnloadObject(handle_);
}

void* SharedLibrary::symbol(const char* n) {
	return SDL_LoadFunction(handle_, n);
}

VOLCANO_END
