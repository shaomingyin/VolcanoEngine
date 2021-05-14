//
//
#ifndef VOLCANO_ZIPFILESYSTEM_HPP
#define VOLCANO_ZIPFILESYSTEM_HPP

#include <string_view>

#include <miniz_zip.h>

#include <Volcano/Common.hpp>
#include <Volcano/FileSystem.hpp>

VOLCANO_BEGIN

class ZipFileSystem: public FileSystem {
public:
	ZipFileSystem(void);
	~ZipFileSystem(void) override;

public:
	bool init(std::string_view nativePath);
	const std::string &nativePath(void) const;
	Type type(std::string_view path) override;
	bool remove(std::string_view path) override;
	bool makeDir(std::string_view dirName) override;
	void enumDir(std::string_view dirName, StringList &out, bool isRecursive) override;
	IO *openFile(std::string_view fileName, int modes) override;

protected:
	bool stat(std::string_view path, mz_zip_archive_file_stat &out);

private:
	mz_zip_archive m_zipArchive;
};

VOLCANO_END

#endif // VOLCANO_ZIPFILESYSTEM_HPP
