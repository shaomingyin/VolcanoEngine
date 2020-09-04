//
//
#include <functional>

#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <QSharedPointer>
#include <QObject>

#include <Volcano/Drivers/Graphics.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

typedef QSharedPointer<Context> ContextPtr;
typedef QMap<QOpenGLContext *, ContextPtr> ContextMap;

static ContextMap contextMap;
static QMutex contextMapMutex;

Context *defaultContext(void)
{
    QMutexLocker locker(&contextMapMutex);

    QOpenGLContext *glContext = QOpenGLContext::currentContext();
    if (glContext == nullptr)
        return nullptr;

    if (contextMap.contains(glContext))
        return contextMap.value(glContext).get();

    ContextPtr p(new Context());
    if (!p || !p->init())
        return nullptr;

    QObject::connect(glContext, &QOpenGLContext::aboutToBeDestroyed, [glContext]() {
        QMutexLocker locker(&contextMapMutex);
        contextMap.remove(glContext);
    });

    contextMap[glContext] = p;

    return p.get();
}

VOLCANO_DRIVERS_GRAPHICS_END
