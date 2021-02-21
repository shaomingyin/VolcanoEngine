//
//
#ifndef VOLCANO_GAME_CONTEXTIMPL_HPP
#define VOLCANO_GAME_CONTEXTIMPL_HPP

#include <QList>
#include <QString>
#include <QStringList>
#include <QPluginLoader>
#include <QResource>
#include <QObject>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/PlayBase.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API ContextImpl: public Context {
    Q_OBJECT

public:
    ContextImpl(QObject *parent = nullptr);
    ~ContextImpl(void) override;

public:
    bool init(const QString &rootDirPath);
    void update(float elapsed);
    const QString &rootDir(void) const override;
    World &world(void) override;

private:
    QString m_rootDir;
    QStringList m_resourceFileNames;
    QPluginLoader m_playLoader;
    PlayBase *m_play;
    World m_world;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXTIMPL_HPP
