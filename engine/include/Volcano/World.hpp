//
//
#ifndef VOLCANO_WORLD_HPP
#define VOLCANO_WORLD_HPP

#include <QList>
#include <QObject>
#include <QQmlListProperty>

#include <Volcano/Common.hpp>
#include <Volcano/Scene.hpp>

VOLCANO_BEGIN

class World: public QObject {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Scene> scenes READ qmlScenes)
    Q_CLASSINFO("DefaultProperty", "scenes")

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    const QList<Scene *> &scenes(void) const;
    QQmlListProperty<Scene> qmlScenes(void);

signals:
    void sceneAdded(Scene *scene);
    void sceneRemoved(Scene *scene);

private:
    static void appendScene(QQmlListProperty<Scene> *list, Scene *scene);
    static qsizetype sceneCount(QQmlListProperty<Scene> *list);
    static Scene *sceneAt(QQmlListProperty<Scene> *list, qsizetype index);
    static void clearScenes(QQmlListProperty<Scene> *list);
    static void replaceScene(QQmlListProperty<Scene> *list, qsizetype index, Scene *scene);
    static void removeLastScene(QQmlListProperty<Scene> *list);

private:
    QList<Scene *> m_scenes;
};

VOLCANO_END

#endif // VOLCANO_WORLD_HPP
