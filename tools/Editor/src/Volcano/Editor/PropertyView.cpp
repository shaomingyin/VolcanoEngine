//
//
#include <Volcano/Editor/PropertyView.h>

VOLCANO_EDITOR_BEGIN

PropertyView::PropertyView(Context& context)
    : Core::NavigationWidget(nullptr, Core::Side::Right)
    , context_(context) {
}

VOLCANO_EDITOR_END
