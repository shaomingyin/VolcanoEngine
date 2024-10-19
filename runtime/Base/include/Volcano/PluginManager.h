//
//
#ifndef VOLCANO_PLUGINMANAGER_H
#define VOLCANO_PLUGINMANAGER_H

#include <memory>
#include <list>

#include <Volcano/Common.h>
#include <Volcano/PluginHolder.h>

VOLCANO_BEGIN

using PluginPtr = std::shared_ptr<PluginHolder>;
using PluginPtrList = std::list<PluginPtr>;

class PluginManager {
public:
	PluginManager();
	virtual ~PluginManager();

public:

private:
	PluginPtrList plugins_;
};

VOLCANO_END

#endif // VOLCANO_PLUGINMANAGER_H
