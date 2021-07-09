//
//
#include <QCoreApplication>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

QJsonObject toJsonValue(QObject *object)
{
    if (object == nullptr)
        return QJsonValue(QJsonValue::Null);

    QJsonObject object;

    auto metaObject = object->metaObject();
    metaObject->className()
}

QObject *fromJson(const QByteArray &json)
{

}

void setApplicationName(const QString &name)
{
    QCoreApplication::setOrganizationName("VolcanoEngine");
    QCoreApplication::setApplicationVersion(VOLCANO_VERSION_STR);
    QCoreApplication::setApplicationName(name);
}

VOLCANO_END
