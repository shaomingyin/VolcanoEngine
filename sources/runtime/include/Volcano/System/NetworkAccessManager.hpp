//
//
#ifndef VOLCANO_SYSTEM_NETWORKACCESSMANAGER_HPP
#define VOLCANO_SYSTEM_NETWORKACCESSMANAGER_HPP

#include <QDir>
#include <QMap>
#include <QList>
#include <QString>
#include <QScopedPointer>
#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <Volcano/ZipReader.hpp>
#include <Volcano/NetworkLoader.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

using NetworkLoaders = QMap<QString, NetworkLoader *>;

class NetworkAccessManager: public QNetworkAccessManager {
    Q_OBJECT

public:
    NetworkAccessManager(QObject *parent = nullptr);
    ~NetworkAccessManager(void) override;

public:
    QStringList supportedSchemes(void) const override;
    bool init(const QString &rootDirPath);
    QString rootDirPath(void) const;
    void setOverlayPaths(const QStringList &v);
    const QStringList &overlayPaths(void) const;
    void setNetworkLoaders(const NetworkLoaders &v);
    const NetworkLoaders &networkLoaders(void) const;

protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &originalReq, QIODevice *outgoingData) override;
    static bool checkVfsPath(const QString &path);

private:

private:
    QDir m_rootDir;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_NETWORKACCESSMANAGER_HPP
