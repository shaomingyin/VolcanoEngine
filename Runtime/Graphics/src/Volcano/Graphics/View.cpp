//
//
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

View::View(QObject* parent)
    : QObject(parent)
    , framing_(false)
    , frame_data_vector_(std::thread::hardware_concurrency()) {
}

VOLCANO_GRAPHICS_END
