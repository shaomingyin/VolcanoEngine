//
//
#include <QScopeGuard>

#include <Volcano/Graphics/Pipeline.h>

VOLCANO_GRAPHICS_BEGIN

Pipeline::Pipeline() {
	enable(BoolAttribute::Clear);
    attr(ColorAttribute::ClearColor) = QColor::fromRgb(0.0f, 0.0f, 0.0f);
}

Pipeline::~Pipeline() {
}

void Pipeline::reset() {
}

void Pipeline::apply() const {

}

VOLCANO_GRAPHICS_END
