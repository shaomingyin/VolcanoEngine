//
//
#include <memory>

#include <QMutexLocker>

#include <Volcano/System/Loopback.hpp>

VOLCANO_SYSTEM_BEGIN

QMutex Loopback::c_mutex;
Loopback::LoopbackMap Loopback::c_loopbackMap;

Loopback::Loopback(const QString &name, QObject *parent):
    QObject(parent)
{
}

Loopback::~Loopback(void)
{
}

LoopbackPtr Loopback::create(const QString &name)
{
    QMutexLocker locker(&c_mutex);

    if (c_loopbackMap.count(name) > 0)
        return nullptr;

    LoopbackPtr loopbackPtr(new Loopback(name));
    if (!loopbackPtr || !loopbackPtr->init())
        return nullptr;

    return loopbackPtr;
}

void Loopback::destory(const QString &name)
{
    QMutexLocker locker(&c_mutex);

    auto it = c_loopbackMap.find(name);
    if (it != c_loopbackMap.end())
        c_loopbackMap.erase(it);
}

LoopbackPtr Loopback::get(const QString &name)
{
    QMutexLocker locker(&c_mutex);

    auto it = c_loopbackMap.find(name);
    if (it != c_loopbackMap.end())
        return it->second;
}

const QString &Loopback::name(void) const
{

}

QIODevice &Loopback::txChannel(void)
{

}

QIODevice &Loopback::rxChannel(void)
{

}

bool Loopback::init(void)
{
    return true;
}

VOLCANO_SYSTEM_END
