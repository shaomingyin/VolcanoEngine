//
//
#ifndef VOLCANO_RESOURCECOMPILER_TOOL_H
#define VOLCANO_RESOURCECOMPILER_TOOL_H

#include <Volcano/ResourceBuilder/Common.h>

VOLCANO_RESOURCEBUILDER_BEGIN

class Tool {
public:
	Tool(const argh::parser& cmdline);
	virtual ~Tool() = default;

public:
	virtual int run() = 0;

private:
	const argh::parser& cmdline_;
};

VOLCANO_RESOURCEBUILDER_END

#endif // VOLCANO_RESOURCECOMPILER_TOOL_H
