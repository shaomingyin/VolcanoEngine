//
//
#ifndef VOLCANO_DIR_HPP
#define VOLCANO_DIR_HPP

#include <string>
#include <string_view>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Dir {
public:
	Dir(void);
	Dir(std::string_view path);
	virtual ~Dir(void);

public:
	const std::string &path(void) const;
	static bool mkdir(std::string_view path);
	static bool isDir(std::string_view path);

private:
	std::string m_path;
};

VOLCANO_END

#endif // VOLCANO_DIR_HPP
