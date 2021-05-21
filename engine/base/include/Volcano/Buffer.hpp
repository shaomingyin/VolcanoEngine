//
//
#ifndef VOLCANO_BUFFER_HPP
#define VOLCANO_BUFFER_HPP

#include <Volcano/Common.hpp>
#include <Volcano/IO.hpp>

VOLCANO_BEGIN

class Buffer : public IO {
public:
	Buffer(void);
	Buffer(int64_t size);
	Buffer(const Buffer &that);
	Buffer(Buffer &&that);
	Buffer(const ByteArray &byteArray);
	Buffer(const uint8_t *p, int64_t len);
	~Buffer(void) override;

public:
	int flags(void) override;
	int64_t size(void) override;
	bool open(int modes) override;
	void close(void) override;
	bool isEof(void) override;
	const ByteArray &byteArray(void) const;

protected:
	int64_t pos(void) override;
	bool setPos(int64_t pos) override;
	int64_t readData(void *buf, int64_t len) override;
	int64_t writeData(const void *buf, int64_t len) override;
	void *createMap(int64_t offset, int64_t len) override;
	void releaseMap(void *p, int64_t len) override;

private:
	ByteArray m_byteArray;
	int64_t m_pos;
};

VOLCANO_END

#endif // VOLCANO_BUFFER_HPP
