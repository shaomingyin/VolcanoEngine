//
//
#ifndef VOLCANO_FILENETWORKREPLY_HPP
#define VOLCANO_FILENETWORKREPLY_HPP

#include <QString>
#include <QNetworkReply>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class FileNetworkReply: public QNetworkReply {
    Q_OBJECT

public:
    FileNetworkReply(const QString &fileName, QObject *parent = nullptr);
    ~FileNetworkReply(void) override;
};

VOLCANO_END

#endif // VOLCANO_FILENETWORKREPLY_HPP
