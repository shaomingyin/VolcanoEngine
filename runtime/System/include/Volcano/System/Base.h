//
//
#ifndef VOLCANO_SYSTEM_BASE_H
#define VOLCANO_SYSTEM_BASE_H

#include <QUrl>
#include <QString>
#include <QFuture>
#include <QObject>
#include <QQmlEngine>
#include <QQmlComponent>

#include <Volcano/World/Scene.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Base: public QObject {
    Q_OBJECT
    Q_PROPERTY(State state READ state NOTIFY stateChanged FINAL)

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
    Base(QQmlEngine* engine, QObject* parent = nullptr);
    ~Base() override;

public:
    void setUrl(const QUrl& url, bool force = false);

    State state() const {
        return state_;
    }

    World::Scene* scene() {
        return scene_;
    }

    const World::Scene* scene() const {
        return scene_;
    }

    virtual void update(Duration elapsed);

signals:
    void stateChanged(State v);

protected:
    void setState(State v) {
        if (state_ != v) {
            state_ = v;
            stateChanged(v);
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
        if (loading_task_.isValid()) {
            return loading_task_.progressText();
        }
        return QString();
    }

    void setErrorMessage(const QString& message) {
        error_message_ = message;
        setState(State::Error);
    }

    const QString& errorMessage() const {
        Q_ASSERT(state_ == State::Error);
        return error_message_;
    }

    virtual void loadScene(World::Scene* scene, QPromise<void>& promise);
    virtual void loadEntity(World::Entity* entity, QPromise<void>& promise);
    virtual void loadComponent(World::Entity* entity, World::Component* component, QPromise<void>& promise);

private slots:
    void onComponentStatusChanged(QQmlComponent::Status st);

private:
    void startLoadScene(World::Scene* scene);

private:
    State state_;
    QQmlComponent component_;
    QFuture<void> loading_task_;
    QString error_message_;
    World::Scene* scene_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_BASE_H
