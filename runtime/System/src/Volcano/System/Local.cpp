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
}

VOLCANO_SYSTEM_END
