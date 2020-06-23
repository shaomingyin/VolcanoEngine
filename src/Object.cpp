//
//
#include <QChildEvent>

#include <Volcano/Object.h>

VOLCANO_BEGIN

Object::Object(QObject *parent):
    QObject(parent),
    m_flags(FlagEnable | FlagVisible)
{
}

Object::~Object(void)
{
}

QMatrix4x4 Object::globalTransform(void)
{
    QMatrix4x4 m(transform());

    QObject *obj = this;
    for (;;)
    {
        Object *parent = qobject_cast<Object *>(obj->parent());
        if (parent == nullptr)
            break;
        m = parent->transform() * m;
        obj = obj->parent();
    }

    return m;
}

VOLCANO_END
