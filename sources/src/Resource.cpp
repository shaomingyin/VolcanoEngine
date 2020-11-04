//
//
#include <Volcano/Resource.hpp>

VOLCANO_BEGIN

Resource::Resource(QObject *parent):
    QNetworkReply(parent)
{
}

Resource::~Resource(void)
{
}

VOLCANO_END
