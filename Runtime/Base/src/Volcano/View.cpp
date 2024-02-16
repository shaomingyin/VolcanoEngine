//
//
#include <thread>

#include <Volcano/View.hpp>

VOLCANO_BEGIN

View::View(QObject* parent)
    : QObject(parent)
    , framing_(false)
    , frame_data_vector_(std::thread::hardware_concurrency()) {
}

VOLCANO_END
