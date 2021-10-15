//
//
#ifndef VOLCANO_SYSTEM_NETWORKACCESSMANAGER_HPP
#define VOLCANO_SYSTEM_NETWORKACCESSMANAGER_HPP

#include <QDir>
#include <QList>
#include <QString>
#include <QScopedPointer>
#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <Volcano/ZipReader.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class NetworkAccessManager: public QNetworkAccessManager {
    Q_OBJECT

public:
    NetworkAccessManager(QObject *parent = nullptr);
    ~NetworkAccessManager(void) override;

public:
    QStringList supportedSchemes(void) const override;
    bool init(const QString &rootDirPath, const QStringList &overlayPathList = QStringList());
    QString rootDirPath(void) const;

protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &originalReq, QIODevice *outgoingData) override;
    static bool checkPath(const QString &path);

private:

private:
    QDir m_rootDir;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_NETWORKACCESSMANAGER_HPP
