//
//
#ifndef VOLCANO_PHYSFILESYSTEM_HPP
#define VOLCANO_PHYSFILESYSTEM_HPP

#include <physfs.h>

#include <Volcano/Common.hpp>
#include <Volcano/FileSystem.hpp>

VOLCANO_BEGIN

class PhysFileSystem: public FileSystem {
public:
	PhysFileSystem(void);
	~PhysFileSystem(void) override;

public:
	Type type(std::string_view path) override;
	bool remove(std::string_view path) override;
	bool makeDir(std::string_view dirName) override;
	void enumDir(std::string_view dirName, StringList &out, bool isRecursive) override;
	IO *openFile(std::string_view fileName, int modes) override;

private:
};

VOLCANO_END

#endif // VOLCANO_PHYSFILESYSTEM_HPP
