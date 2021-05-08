//
//
#ifndef VOLCANO_ROOTFILESYSTEM_HPP
#define VOLCANO_ROOTFILESYSTEM_HPP

#include <list>
#include <memory>
#include <string>
#include <string_view>

#include <Volcano/Common.hpp>
#include <Volcano/FileSystem.hpp>

VOLCANO_BEGIN

class RootFileSystem : public FileSystem {
public:
	RootFileSystem(void);
	~RootFileSystem(void) override;

public:
	bool init(std::string_view nativePath) override;
	Type type(std::string_view path) override;
	void enumDir(std::string_view dirName, StringList &out, bool isRecursive) override;
	IO *openFile(std::string_view fileName, int modes) override;
	bool mount(std::string_view nativePath, std::string_view path);
	bool umount(std::string_view nativePath, std::string_view path);

protected:
	using FileSystemPtr = std::unique_ptr<FileSystem>;

	struct MountInfo {
		FileSystemPtr fs;
		std::string path;
	};

	using MountInfoList = std::list<MountInfo>;

private:
	MountInfoList m_mountInfos;
};

VOLCANO_END

#endif // VOLCANO_ROOTFILESYSTEM_HPP
