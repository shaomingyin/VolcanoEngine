//
//
#include <QMetaObject>
#include <QTextStream>

#include <Volcano/Editor/Qmlizer.h>

VOLCANO_EDITOR_BEGIN

Qmlizer::Qmlizer(QObject& target, QObject *parent)
    : QObject(parent)
    , target_(target) {
}

QString Qmlizer::toString() const {
    QString qml;
    QTextStream s(&qml);
    const QMetaObject* meta_obj = target_.metaObject();
    s << meta_obj->className() << "{\n";
    s << "}";
    return qml;
}

VOLCANO_EDITOR_END
