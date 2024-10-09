//
//
#ifndef VOLCANO_PLUGINHOLDER_H
#define VOLCANO_PLUGINHOLDER_H

#include <string>

#include <Volcano/Common.h>
#include <Volcano/Plugin.h>

VOLCANO_BEGIN

class PluginHolder final {
public:
	PluginHolder(const std::string& path);
	~PluginHolder();

public:
	bool open();
	void close();

private:
	std::string name_;
};

VOLCANO_END

#endif // VOLCANO_PLUGINHOLDER_H
