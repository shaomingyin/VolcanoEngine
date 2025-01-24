//
//
#include <Volcano/System/Local.h>

VOLCANO_SYSTEM_BEGIN

Local::Local(QQmlEngine* engine, const QUrl& url, QObject* parent)
    : Base(engine, url, parent)
    , view_current_(0)
    , potential_visible_set_builder_(scene()) {
}

void Local::update(Duration elapsed) {
    Base::update(elapsed);

    bool threaded = thread()->isCurrentThread();
    if (threaded) {

    } else {

    }

    potential_visible_set_builder_.build(view_[!view_current_]);

    if (threaded) {

    } else {

    }
}

void Local::render() {
    renderer_.render(view_[view_current_]);
}

VOLCANO_SYSTEM_END
