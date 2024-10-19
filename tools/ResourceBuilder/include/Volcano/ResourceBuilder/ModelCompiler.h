//
//
#ifndef VOLCANO_RESOURCECOMPILER_MODELCOMPILER_H
#define VOLCANO_RESOURCECOMPILER_MODELCOMPILER_H

#include <Volcano/ResourceBuilder/Common.h>
#include <Volcano/ResourceBuilder/Tool.h>

VOLCANO_RESOURCEBUILDER_BEGIN

class ModelCompiler: public Tool {
public:
	ModelCompiler(const argh::parser& cmldine);
	virtual ~ModelCompiler() = default;

public:
	int run() override;
};

VOLCANO_RESOURCEBUILDER_END

#endif // VOLCANO_RESOURCECOMPILER_MODELCOMPILER_H
