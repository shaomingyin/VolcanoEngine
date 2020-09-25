//
//
#ifndef VOLCANO_RESOURCE_HPP
#define VOLCANO_RESOURCE_HPP

#include <QNetworkReply>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API Resource: public QNetworkReply
{
    Q_OBJECT

public:
    Resource(QObject *parent = nullptr);
    ~Resource(void) override;
};

VOLCANO_END

#endif // VOLCANO_RESOURCE_HPP
