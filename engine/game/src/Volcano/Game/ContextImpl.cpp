//
//
#include <QDir>
#include <QScopeGuard>

#include <Volcano/Game/ContextImpl.hpp>

VOLCANO_GAME_BEGIN

ContextImpl::ContextImpl(QObject *parent):
    Context(parent),
    m_play(nullptr)
{
}

ContextImpl::~ContextImpl(void)
{
    if (m_play != nullptr)
        m_play->shutdown();

    if (m_playLoader.isLoaded())
        m_playLoader.unload();

    for (int i = 0; i < m_resourceFileNames.size(); ++i)
        QResource::unregisterResource(m_resourceFileNames.at(i));
}

bool ContextImpl::init(const QString &rootDirPath)
{
    QDir rootDir(rootDirPath);
    if (!rootDir.exists())
        return false;

    QDir resourceDir(rootDir.absolutePath() + "/resources");
    if (resourceDir.exists()) {
        QStringList resourceFilter;
        resourceFilter << "*.rcc";
        m_resourceFileNames = resourceDir.entryList(resourceFilter, QDir::Files | QDir::Readable);
        for (int i = 0; i < m_resourceFileNames.size(); ++i)
            QResource::registerResource(m_resourceFileNames.at(i), "/play");
    }

    QScopeGuard unregisterResourceGuard([&]() {
        for (int i = 0; i < m_resourceFileNames.size(); ++i)
            QResource::unregisterResource(m_resourceFileNames.at(i));
    });

    m_playLoader.setFileName(rootDir.absolutePath() + "/main");
    QObject *playObject = m_playLoader.instance();
    if (playObject == nullptr)
        return false;

    QScopeGuard unloadPluginGuard([&]() { m_playLoader.unload(); });

    m_play = qobject_cast<PlayBase *>(playObject);
    if (m_play == nullptr || !m_play->init(this))
        return false;

    m_rootDir = rootDir.absolutePath();

    return true;
}

void ContextImpl::update(float elapsed)
{
    if (Q_LIKELY(m_play != nullptr)) {
        m_world.update(elapsed);
        m_play->update(elapsed);
    }
}

const QString &ContextImpl::rootDir(void) const
{
    return m_rootDir;
}

World &ContextImpl::world(void)
{
    return m_world;
}

VOLCANO_GAME_END
