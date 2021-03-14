//
//
#ifndef VOLCANO_FILESYSTEM_HPP
#define VOLCANO_FILESYSTEM_HPP

#include <string_view>

#include <Volcano/Common.hpp>
#include <Volcano/IO.hpp>

#define VOLCANO_FILESYSTEM_BEGIN VOLCANO_BEGIN namespace FileSystem {
#define VOLCANO_FILESYSTEM_END } VOLCANO_END

VOLCANO_FILESYSTEM_BEGIN

enum class EntryType {
	Invalid = 0,
	Dir,
	File
};

extern EntryType entryType(std::string_view path);
extern IO *getFile(std::string_view fileName, int ioMode);
extern bool enumDir(std::string_view dirName, StringList &entryList);
extern std::string_view currentDir(void);
extern bool setCurrentDir(std::string_view dirName);

VOLCANO_FILESYSTEM_END

#endif // VOLCANO_FILESYSTEM_HPP
