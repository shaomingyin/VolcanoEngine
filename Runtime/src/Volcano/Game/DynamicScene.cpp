//
//
#include <Volcano/Game/DynamicScene.hpp>

VOLCANO_GAME_BEGIN

DynamicScene::DynamicScene(QObject* parent)
    : Scene(parent)
    , gravity_(0.0f, -9.8f, 0.0f) {
    // TODO create bt world.
}

DynamicScene::DynamicScene(Context& context, QObject* parent)
    : Scene(context, parent)
    , gravity_(0.0f, -9.8f, 0.0f) {
    // TODO create bt world.
}

const QVector3D &DynamicScene::gravity(void) const {
    return gravity_;
}

void DynamicScene::setGravity(const QVector3D &v) {
    if (!qFuzzyCompare(gravity_, v)) {
        gravity_ = v;
        if (btDynamicsWorld_) {
            btDynamicsWorld_->setGravity({ v.x(), v.y(), v.z() });
        }
        emit gravityChanged(v);
    }
}

VOLCANO_GAME_END
