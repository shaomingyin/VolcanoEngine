//
//
#ifndef VOLCANO_STREAM_H
#define VOLCANO_STREAM_H

#include <Volcano/Common.h>

VOLCANO_BEGIN

class Stream {
public:
	Stream() = default;
	virtual ~Stream() = default;

public:
	virtual size_t size() = 0;
	virtual size_t position() = 0;
};

class InputStream: public Stream {
public:
	InputStream() = default;
	virtual ~InputStream() = default;

public:
	virtual size_t read(void* buffer, size_t size) = 0;
};

class OutputStream : public Stream {
public:
	OutputStream() = default;
	virtual ~OutputStream() = default;

public:
	virtual size_t write(const void* buffer, size_t size) = 0;
};

VOLCANO_END

#endif // VOLCANO_STREAM_H
