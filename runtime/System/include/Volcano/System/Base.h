//
//
#ifndef VOLCANO_SYSTEM_BASE_H
#define VOLCANO_SYSTEM_BASE_H

#include <QString>
#include <QEvent>
#include <QFuture>
#include <QObject>

#include <Volcano/World/Scene.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Base: public QObject {
    Q_OBJECT

public:
    enum class State {
        Idle = 0,
        Loading,
        Ready,
        Playing,
        Paused,
        Stopping,
        Error
    };

public:
    Base(World::Scene& scene, QObject* parent = nullptr);
    ~Base() override;

public:
    State state() const {
        return state_;
    }

    int fps() const {
        return frame_count_per_second_;
    }

    int fpsMax() const {
        return (1000000000 / std::chrono::duration_cast<std::chrono::nanoseconds>(frame_elapsed_min_).count());
    }

    void setFpsMax(int v) {
        frame_elapsed_min_ = std::chrono::nanoseconds(1000000000 / std::clamp(v, 1, 999));
    }

    World::Scene& scene() {
        return scene_;
    }

    const World::Scene& scene() const {
        return scene_;
    }

    virtual void start();

signals:
    void stateChanged(State v);

protected:
    void setState(State v) {
        if (state_ != v) {
            state_ = v;
            emit stateChanged(v);
        }
    }

    int loadingProgress() const {
        Q_ASSERT(state_ == State::Loading);
        if (loading_task_.isValid()) {
            auto n = loading_task_.progressMaximum() - loading_task_.progressMinimum();
            if (n < 1) {
                n = 1;
            }
            auto v = loading_task_.progressValue() - loading_task_.progressMinimum();
            return (v * 100 / n);
        }
        return 0;
    }

    QString loadingMessage() const {
        Q_ASSERT(state_ == State::Loading);
        QString message;
        if (loading_task_.isValid()) {
            message = loading_task_.progressText();
        }
        return message;
    }

    void setErrorMessage(QString&& message) {
        error_message_ = std::move(message);
        setState(State::Error);
    }

    const QString& errorMessage() const {
        Q_ASSERT(state_ == State::Error);
        return error_message_;
    }

    virtual void loadComponent(World::Entity* entity, World::Component* component, QPromise<void>& promise);
    virtual void update(Duration elapsed);

private:
    void load(QPromise<void>& promise);
    void frame();

private:
    World::Scene& scene_;
    State state_;
    QFuture<void> loading_task_;
    QString error_message_;
    Duration frame_elapsed_min_;
    TimePoint frame_last_;
    TimePoint frame_count_last_;
    int frame_count_;
    int frame_count_per_second_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_BASE_H
