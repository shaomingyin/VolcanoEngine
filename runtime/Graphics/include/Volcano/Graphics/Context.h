//
//
#ifndef VOLCANO_GRAPHICS_CONTEXT_H
#define VOLCANO_GRAPHICS_CONTEXT_H

#include <mutex>

#include <QList>
#include <QFuture>
#include <QObject>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Buffer.h>

VOLCANO_GRAPHICS_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(QObject* parent = nullptr);
    virtual ~Context() = default;

public:
    template <typename T>
    QFuture<T> asyncLoad(std::function<T ()> fn) {
        QPromise<T> promise;
        QFuture<T> future = promise.future();
        std::unique_lock<std::mutex> lg(async_load_task_mutex_);
        async_load_tasks_.emplace([fn = std::move(fn), promise = std::move(promise)] {
            promise.start();
            try {
                fn(promise);
            } catch (std::exception& ex) {
                promise.setException(ex);
            }
            promise.finish();
        });
    }

    virtual Buffer* allocStaticVertexBuffer(GLsizei size) = 0;
    virtual Buffer* allocStaticIndexBuffer(GLsizei size) = 0;

protected:
    void runAsyncLoadTasks() {
        std::unique_lock<std::mutex> lg(async_load_task_mutex_);
    }

private:
    std::mutex async_load_task_mutex_;
    QList<std::function<void ()>> async_load_tasks_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CONTEXT_H
