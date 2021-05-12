//
//
#ifndef VOLCANO_NATIVEFILESYSTEM_HPP
#define VOLCANO_NATIVEFILESYSTEM_HPP

#include <string>
#include <string_view>

#include <Volcano/Common.hpp>
#include <Volcano/FileSystem.hpp>

VOLCANO_BEGIN

class NativeFileSystem: public FileSystem {
public:
	NativeFileSystem(void);
	~NativeFileSystem(void) override;

public:
	bool init(std::string_view nativePath);
	const std::string &nativePath(void) const;
	Type type(std::string_view path) override;
	bool remove(std::string_view path) override;
	bool makeDir(std::string_view dirName) override;
	void enumDir(std::string_view dirName, StringList &out, bool isRecursive) override;
	IO *openFile(std::string_view fileName, int modes) override;

private:
	std::string m_nativePath;
};

VOLCANO_END

#endif // VOLCANO_NATIVEFILESYSTEM_HPP
