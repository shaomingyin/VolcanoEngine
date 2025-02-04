//
//
#include <QThread>
#include <QOpenGLContext>

#include <Volcano/System/Local.h>

VOLCANO_SYSTEM_BEGIN

Local::Local(World::Scene& scene, QObject* parent)
    : Base(scene, parent) {
}

bool Local::event(QEvent* evt) {
    for (auto& screen_controller: screen_controller_list_) {
        if (screen_controller.event(evt)) {
            return true;
        }
    }
    return Base::event(evt);
}

void Local::update(Duration elapsed) {
    Base::update(elapsed);
    Graphics::View view;
    addScreensToView(view);
    addSceneToView(view);
    renderer_.render(view);
}

void Local::addScreensToView(Graphics::View& view) const {
    QtConcurrent::map(scene().entities(), [this](auto& entity) {

    }).waitForFinished();
}

void Local::addSceneToView(Graphics::View& view) const {
    QtConcurrent::map(screen_controller_list_, [this](auto& screen_controller) {

    }).waitForFinished();
}

VOLCANO_SYSTEM_END
