//
//
#ifndef VOLCANO_FILESYSTEM_HPP
#define VOLCANO_FILESYSTEM_HPP

#include <string_view>

#include <Volcano/Common.hpp>
#include <Volcano/IO.hpp>

VOLCANO_BEGIN

class FileSystem {
	VOLCANO_DISABLE_COPY_AND_MOVE(FileSystem)

public:
	enum class Type {
		Unknown = 0,
		Dir,
		File
	};

public:
	FileSystem(void) = default;
	virtual ~FileSystem(void) = default;

public:
	virtual Type type(std::string_view path) = 0;
	bool exists(std::string_view path);
	virtual bool remove(std::string_view path) = 0;
	virtual bool makeDir(std::string_view dirName) = 0;
	virtual void enumDir(std::string_view dirName, StringList &out, bool isRecursive = false) = 0;
	virtual IO *openFile(std::string_view fileName, int modes) = 0;
	static bool checkPath(std::string_view path);
	static bool isNameChar(char c);
};

VOLCANO_END

#endif // VOLCANO_FILESYSTEM_HPP
