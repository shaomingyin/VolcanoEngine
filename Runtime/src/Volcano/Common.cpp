//
//
#include <QCoreApplication>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

void setApplicationName(const QString &name)
{
    QCoreApplication::setOrganizationName("VolcanoEngine");
    QCoreApplication::setApplicationVersion(VOLCANO_VERSION_STR);
    QCoreApplication::setApplicationName(name);
}

VOLCANO_END
