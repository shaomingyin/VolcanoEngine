//
//
#ifndef VOLCANO_SYSTEM_FILENETWORKREPLY_HPP
#define VOLCANO_SYSTEM_FILENETWORKREPLY_HPP

#include <QString>
#include <QNetworkReply>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class FileNetworkReply: public QNetworkReply {
    Q_OBJECT

public:
    FileNetworkReply(const QString &fileName, QObject *parent = nullptr);
    ~FileNetworkReply(void) override;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_FILENETWORKREPLY_HPP
