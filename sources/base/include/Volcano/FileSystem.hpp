//
//
#ifndef VOLCANO_FILESYSTEM_HPP
#define VOLCANO_FILESYSTEM_HPP

#include <string>
#include <string_view>

#include <Volcano/Common.hpp>
#include <Volcano/IO.hpp>

VOLCANO_BEGIN

class FileSystem: public Noncopyable {
public:
	enum class Type {
		Invalid = 0,
		Dir,
		File
	};

public:
	FileSystem(void);
	virtual ~FileSystem(void);

public:
	virtual bool isWritable(void);
	virtual bool init(std::string_view nativePath);
	const std::string &nativePath(void) const;
	bool isExists(std::string_view path);
	virtual Type type(std::string_view path);
	virtual bool remove(std::string_view path);
	virtual bool makeDir(std::string_view dirName);
	virtual void enumDir(std::string_view dirName, StringList &out, bool isRecursive = false);
	virtual IO *openFile(std::string_view fileName, int modes);
	static bool isPath(std::string_view path);
	static bool isNameChar(char path);

private:
	std::string m_nativePath;
};

VOLCANO_END

#endif // VOLCANO_FILESYSTEM_HPP
