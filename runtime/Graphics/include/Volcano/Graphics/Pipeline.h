//
//
#ifndef VOLCANO_GRAPHICS_PIPELINE_H
#define VOLCANO_GRAPHICS_PIPELINE_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Context.h>

VOLCANO_GRAPHICS_BEGIN

class Pipeline {
public:
	Pipeline(Context& context);

private:
	Context& context_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PIPELINE_H
