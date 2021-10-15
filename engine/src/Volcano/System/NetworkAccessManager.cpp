//
//
#include <QChar>

#include <Volcano/System/NetworkAccessManager.hpp>

VOLCANO_SYSTEM_BEGIN

NetworkAccessManager::NetworkAccessManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
}

NetworkAccessManager::~NetworkAccessManager(void)
{
}

QStringList NetworkAccessManager::supportedSchemes(void) const
{
    qDebug() << __FUNCTION__;
    return (QNetworkAccessManager::supportedSchemes() << "vfs");
}

bool NetworkAccessManager::init(const QString &rootDirPath, const QStringList &overlayPathList)
{
    QFileInfo info(rootDirPath);
    if (!info.isDir())
        return false;

    m_rootDir = rootDirPath;

#if 0 // TODO
    for (auto path: overlayPathList) {
        QFileInfo info(path);
        if (findOverlay(info) != m_overlayPtrList.end())
            continue;
        OverlayPtr ol;
        if (info.isFile()) {
            auto zol = new ZipOverlay(info);
            if (!zol || !zol->init())
                continue;
            ol.reset(zol);
        } else if (info.isDir()) {
            ol.reset(new Overlay(info));
            if (!ol)
                continue;
        } else {
            // Nothing to do.
        }
        m_overlayPtrList.append(ol);
    }
#endif

    return true;
}

QString NetworkAccessManager::rootDirPath(void) const
{
    return m_rootDir.path();
}

QNetworkReply *NetworkAccessManager::createRequest(Operation op, const QNetworkRequest &originalReq, QIODevice *outgoingData)
{
    auto url = originalReq.url();
    qDebug() << __FUNCTION__ << url;
    if (url.scheme() != "vfs")
        return QNetworkAccessManager::createRequest(op, originalReq, outgoingData);

    auto path = url.path();
    qDebug() << __FUNCTION__ << path;
    if (!checkPath(path))
        return nullptr;

    QString fullPath(m_rootDir.path() + path);
    qDebug() << __FUNCTION__ << fullPath;
    QFileInfo info(fullPath);
    if (info.isFile()) {
        qDebug() << __FUNCTION__ << QUrl::fromLocalFile(fullPath);
        return QNetworkAccessManager::createRequest(op,
            QNetworkRequest(QUrl::fromLocalFile(fullPath)), outgoingData);
    }

#if 0 // TODO
    for (const auto &overlayPtr: m_overlayPtrList) {
        if (overlayPtr->m_info.isDir()) {
            QString fullPath(overlayPtr->m_info.absoluteFilePath() + path);
            QFileInfo info(fullPath);
            if (info.isFile()) {
                return QNetworkAccessManager::createRequest(op,
                    QNetworkRequest(QUrl::fromLocalFile(fullPath)), outgoingData);
            }
        }

    }
#endif

    return nullptr;
}

bool NetworkAccessManager::checkPath(const QString &path)
{
    if (path.isEmpty())
        return false;

    if (path.at(0) != '/')
        return false;

    bool slashed = true;
    auto n = path.size();

    for (qsizetype i = 1; i < n; ++i) {
        QChar c(path.at(i));
        if (c == '/') {
            if (slashed)
                return false;
            slashed = true;
        } else if (!c.isLetterOrNumber() && c != '.' && c != '_')
            return false;
    }

    return true;
}

#if 0 // TODO
NetworkAccessManager::OverlayPtrList::iterator NetworkAccessManager::findOverlay(const QFileInfo &info)
{
    for (OverlayPtrList::iterator it(m_overlayPtrList.begin()); it != m_overlayPtrList.end(); ++it) {
        if ((*it)->m_info == info)
            return it;
    }

    return m_overlayPtrList.end();
}
#endif

VOLCANO_SYSTEM_END
