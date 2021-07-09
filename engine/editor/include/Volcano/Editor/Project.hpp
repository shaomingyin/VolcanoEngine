//
//
#ifndef VOLCANO_EDITOR_PROJECT_HPP
#define VOLCANO_EDITOR_PROJECT_HPP

#include <QDir>
#include <QString>
#include <QJsonObject>
#include <QFileSystemModel>

#include <Volcano/Game/World.hpp>
#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/WorldModel.hpp>

VOLCANO_EDITOR_BEGIN

class Project: public QObject {
    Q_OBJECT

public:
    Project(QObject *parent = nullptr);
    ~Project(void) override;

public:
    bool init(const QString &rootDirName, const QString &name);
    bool load(const QString &rootDirName);
    bool save(const QString &rootDirName = QString());
    const QString &name(void) const;
    void setName(const QString &v);
    const QDir &rootDir(void) const;
    const QDir &resourcesDir(void) const;
    WorldModel &worldModel(void);
    QFileSystemModel &resourcesModel(void);

signals:
    void nameChanged(const QString &name);

private:
    QDir m_rootDir;
    QDir m_resourcesDir;
    QString m_name;
    Game::World *m_gameWorld;
    WorldModel m_worldModel;
    QFileSystemModel m_resourcesModel;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECT_HPP
