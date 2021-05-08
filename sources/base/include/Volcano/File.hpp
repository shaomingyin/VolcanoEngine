//
//
#ifndef VOLCANO_FILE_HPP
#define VOLCANO_FILE_HPP

#include <cstdio>
#include <string>
#include <string_view>

#include <Volcano/Common.hpp>
#include <Volcano/IO.hpp>

VOLCANO_BEGIN

class File: public IO {
public:
	File(std::string_view fileName);
	~File(void) override;

public:
	int flags(void) override;
	int64_t size(void) override;
	bool open(int modes) override;
	void close(void) override;
	const std::string &fileName(void) const;

protected:
	int64_t pos(void) override;
	bool setPos(int64_t pos) override;
	int64_t readData(void *buf, int64_t len) override;
	int64_t writeData(const void *buf, int64_t len) override;
	void *createMap(int64_t offset, int64_t len) override;
	void releaseMap(void *p, int64_t len) override;

private:
	std::string m_fileName;
	FILE *m_fp;
};

VOLCANO_END

#endif // VOLCANO_FILE_HPP
