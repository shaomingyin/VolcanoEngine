//
//
#ifndef VOLCANO_SDL_INITIALIZER_H
#define VOLCANO_SDL_INITIALIZER_H

#include <Volcano/Common.h>

VOLCANO_BEGIN

struct SDL_Initializer final {
	int init_result;

	SDL_Initializer(Uint32 flags = SDL_INIT_EVERYTHING)
		: init_result(SDL_Init(flags)) {
	}

	~SDL_Initializer() {
		if (init_result == 0) {
			SDL_Quit();
		}
	}
};

VOLCANO_END

#endif // VOLCANO_SDL_INITIALIZER_H
