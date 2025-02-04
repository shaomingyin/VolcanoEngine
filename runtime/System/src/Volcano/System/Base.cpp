//
//
#include <QTimer>
#include <QScopeGuard>

#include <Volcano/World/Screen.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

Base::Base(World::Scene& scene, QObject* parent)
    : QObject(parent)
    , scene_(scene)
    , state_(State::Idle) {
}

Base::~Base() {
    if (loading_task_.isRunning()) {
        loading_task_.cancel();
        loading_task_.waitForFinished();
    }
}

void Base::start() {
    if (state_ != State::Idle) {
        return;
    }

    frame_count_ = 0;
    frame_count_per_second_ = 0;
    frame_last_ = Clock::now();
    frame_count_last_ = frame_last_;

    QTimer::singleShot(0, Qt::PreciseTimer, this, &Base::frame);

    setState(State::Loading);

    if (loading_task_.isRunning()) {
        loading_task_.cancel();
        loading_task_.waitForFinished();
    }

    loading_task_ = QtConcurrent::run([this](QPromise<void>& promise) {
        Base::load(promise);
    });
}

void Base::loadComponent(World::Entity* entity, World::Component* component, QPromise<void>& promise) {
    auto screen = qobject_cast<World::Screen*>(component);
    if (screen != nullptr) {

    }
}

void Base::update(Duration elapsed) {
    if (state_ == State::Playing) {
        scene_.update(elapsed);
    }
}

void Base::load(QPromise<void>& promise) {
    promise.start();
    auto promise_guard = qScopeGuard([this, &promise] {
        if (state_ == State::Loading) {
            setState(State::Ready);
        }
        promise.finish();
    });

    auto& entities = scene_.entities();

    promise.setProgressRange(0, entities.count());
    for (int i = 0; i < entities.count(); ++i) {
        if (promise.isCanceled()) {
            setState(State::Idle);
            break;
        }
        auto entity = entities.at(i);
        promise.setProgressValueAndText(i, QString("Loading entity %1...").arg(i + 1));
        auto& components = entity->components();
        for (auto& component: components) {
            loadComponent(entity, component, promise);
        }
    }
}

void Base::frame() {
    auto curr = Clock::now();
    auto elapsed = curr - frame_last_;
    update(elapsed);
    frame_count_ += 1;
    frame_last_ = curr;
    curr = Clock::now();
    auto frame_elapsed = curr - frame_last_;
    if (frame_elapsed < frame_elapsed_min_) {
        QTimer::singleShot(frame_elapsed_min_ - frame_elapsed, Qt::PreciseTimer, this, &Base::frame);
    } else {
        QTimer::singleShot(0, Qt::PreciseTimer, this, &Base::frame);
    }
    if ((curr - frame_count_last_) >= 1s) {
        frame_count_per_second_ = frame_count_;
        frame_count_ = 0;
        frame_count_last_ = curr;
    }
}

VOLCANO_SYSTEM_END
