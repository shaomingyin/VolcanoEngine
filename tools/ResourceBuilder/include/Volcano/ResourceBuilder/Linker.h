//
//
#ifndef VOLCANO_RESOURCECOMPILER_LINKER_H
#define VOLCANO_RESOURCECOMPILER_LINKER_H

#include <Volcano/ResourceBuilder/Common.h>
#include <Volcano/ResourceBuilder/Tool.h>

VOLCANO_RESOURCEBUILDER_BEGIN

class Linker: public Tool {
public:
	Linker(const argh::parser& cmdline);
	virtual ~Linker() = default;

public:
	int run() override;
};

VOLCANO_RESOURCEBUILDER_END

#endif // VOLCANO_RESOURCECOMPILER_LINKER_H
