//
//
#ifndef VOLCANO_PLUGIN_H
#define VOLCANO_PLUGIN_H

#include <string>

#include <Volcano/Common.h>

VOLCANO_BEGIN

typedef int (*VolcanoPluginInitFunc)();
typedef void (*VolcanoPluginExitFunc)();

VOLCANO_END

#define VOLCANO_PLUGIN_INIT_FUNC_NAME volcanoPluginInit
#define VOLCANO_PLUGIN_EXIT_FUNC_NAME volcanoPluginExit

#define VOLCANO_PLUGIN_INIT_FUNC_TEXT VOLCANO_STRIZE(VOLCANO_PLUGIN_INIT_FUNC_NAME)
#define VOLCANO_PLUGIN_EXIT_FUNC_TEXT VOLCANO_STRIZE(VOLCANO_PLUGIN_EXIT_FUNC_NAME)

#define VOLCANO_PLUGIN_INIT(init_func) \
	VOLCANO_EXPORT VOLCANO_C int VOLCANO_PLUGIN_INIT_FUNC_NAME() { \
		return init_func(); \
	}

#define VOLCANO_PLUGIN_EXIT(exit_func) \
	VOLCANO_EXPORT VOLCANO_C void VOLCANO_PLUGIN_EXIT_FUNC_NAME() { \
		exit_func(); \
	}

#define VOLCANO_PLUGIN(init_func, exit_func) \
	VOLCANO_PLUGIN_INIT(init_func) \
	VOLCANO_PLUGIN_EXIT(exit_func)

#endif // VOLCANO_PLUGIN_H
