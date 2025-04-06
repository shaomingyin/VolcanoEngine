//
//
#ifndef VOLCANO_SHAREDLIBRARY_H
#define VOLCANO_SHAREDLIBRARY_H

#include <string>

#include <Volcano/Common.h>

VOLCANO_BEGIN

class SharedLibrary {
public:
	SharedLibrary(const char* path);
	virtual ~SharedLibrary();

public:
	void* symbol(const char* n);

private:
	SDL_SharedObject* handle_;
};

VOLCANO_END

#endif // VOLCANO_SHAREDLIBRARY_H
