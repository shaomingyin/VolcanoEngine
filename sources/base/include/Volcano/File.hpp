//
//
#ifndef VOLCANO_FILE_HPP
#define VOLCANO_FILE_HPP

#include <string>
#include <string_view>

#include <Volcano/Common.hpp>
#include <Volcano/IO.hpp>

VOLCANO_BEGIN

class FileBase: public IO {
public:
	using Stat = PHYSFS_Stat;

public:
	FileBase(void);
	FileBase(std::string_view path);
	~FileBase(void) override;

public:
	int64_t size(void) override;
	static int64_t size(std::string_view path);
	void close(void) override;
	bool isEof(void) override;
	const std::string &path(void) const;
	void setPath(std::string_view path);
	bool stat(Stat &out);
	static bool stat(std::string_view path, Stat &out);
	static bool isFile(std::string_view path);

protected:
	int64_t pos(void) override;
	bool setPos(int64_t pos) override;
	int64_t readData(void *buf, int64_t len) override;
	int64_t writeData(const void *buf, int64_t len) override;

protected:
	std::string m_path;
	PHYSFS_File *m_handle;
};

class FileReader: public FileBase {
public:
	FileReader(void);
	FileReader(std::string_view &path);
	~FileReader(void) override;

public:
	int flags(void) override;
	bool open(int mode) override;

protected:
    int64_t readData(void *buf, int64_t len) override;
    int64_t writeData(const void *buf, int64_t len) override;
};

class FileWriter: public FileBase {
public:
	FileWriter(void);
	FileWriter(std::string_view &path);
	~FileWriter(void) override;

public:
	int flags(void) override;
	bool open(int mode) override;

protected:
    int64_t readData(void *buf, int64_t len) override;
    int64_t writeData(const void *buf, int64_t len) override;
};

VOLCANO_END

#endif // VOLCANO_FILE_HPP
