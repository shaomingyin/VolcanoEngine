//
//
#ifndef VOLCANO_SYSTEM_NETWORKACCESSMANAGERFACTORY_HPP
#define VOLCANO_SYSTEM_NETWORKACCESSMANAGERFACTORY_HPP

#include <QString>
#include <QStringList>
#include <QQmlNetworkAccessManagerFactory>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class NetworkAccessManagerFactory: public QQmlNetworkAccessManagerFactory {
public:
    NetworkAccessManagerFactory(const QString &rootDir, const QStringList &overlayList);
    ~NetworkAccessManagerFactory(void) override = default;

public:
    QNetworkAccessManager *create(QObject *parent) override;

private:
    QString m_rootDir;
    QStringList m_overlayList;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_NETWORKACCESSMANAGERFACTORY_HPP
