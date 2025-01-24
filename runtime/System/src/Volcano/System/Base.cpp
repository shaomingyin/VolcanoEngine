//
//
#include <QScopeGuard>

#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

Base::Base(QQmlEngine* engine, QObject* parent)
    : QObject(parent)
    , state_(State::Idle)
    , component_(engine) {
}

Base::~Base() {
    if (loading_task_.isRunning()) {
        loading_task_.cancel();
        loading_task_.waitForFinished();
    }
}

void Base::setUrl(const QUrl& url, bool force) {
    if (state_ == State::Loading) {
        return;
    }

    if (component_.url() == url && !force) {
        return;
    }

    setState(State::Loading);
    scene_ = nullptr;

    component_.loadUrl(url);
    if (component_.isReady()) {
        onComponentStatusChanged(QQmlComponent::Ready);
    } else {
        connect(&component_, &QQmlComponent::statusChanged, this, &Base::onComponentStatusChanged);
    }
}

void Base::update(Duration elapsed) {
    if (state_ == State::Playing && scene_ != nullptr) {
        scene_->update(elapsed);
    }
}

void Base::loadScene(World::Scene* scene, QPromise<void>& promise) {
    Q_ASSERT(scene != nullptr);

    auto& entities = scene->entities();
    promise.setProgressRange(0, 1, entities.count())

    if (promise.isCanceled()) {
        setErrorMessage("Cancelled.");
        return;
    }

    promise.setProgressRange(0, entities.count());
    for (int i = 0; i < entities.count(); ++i) {
        promise.setProgressValueAndText(i, QString("Loading entity %1...").arg(i + 1));
        loadEntity(entities.at(i), promise);
    }
}

void Base::loadEntity(World::Entity* entity, QPromise<void>& promise) {
    auto& components = entity->components();
    for (auto& component: components) {
        loadComponent(entity, component, promise);
    }
}

void Base::loadComponent(World::Entity* entity, World::Component* component, QPromise<void>& promise) {
}

void Base::onComponentStatusChanged(QQmlComponent::Status st) {
    switch (st) {
    case QQmlComponent::Ready:
        startLoadScene(qobject_cast<World::Scene*>(component_.create()));
        break;
    case QQmlComponent::Error:
        setErrorMessage(component_.errorString());
        break;
    default:
        break;
    }
}

void Base::startLoadScene(World::Scene* scene) {
    Q_ASSERT(scene_ == nullptr);

    if (scene == nullptr) {
        setErrorMessage("Invalid scene to load.");
        return;
    }

    auto load = [this, scene] {
        loading_task_ = QtConcurrent::run([this, scene](QPromise<void>& promise) {
            promise.start();
            auto promise_guard = qScopeGuard([this, &promise] {
                if (state_ == State::Loading) {
                    setState(State::Ready);
                }
                promise.finish();
            });
            loadScene(scene, promise);
        });
    };

    if (loading_task_.isRunning()) {
        loading_task_.onCanceled(std::move(load));
        loading_task_.cancel();
    } else {
        load();
    }
}

VOLCANO_SYSTEM_END
