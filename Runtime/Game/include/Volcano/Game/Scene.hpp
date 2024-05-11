//
//
#ifndef VOLCANO_GAME_SCENE_HPP
#define VOLCANO_GAME_SCENE_HPP

#include <QMap>
#include <QString>
#include <QScopedPointer>
#include <QVector3D>
#include <QObject>
#include <QQmlListProperty>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

using ActorList = QList<Actor*>;
using EntityList = QList<Entity*>;
using DirectionalLightList = QList<DirectionalLight*>;
using SpotLightList = QList<SpotLight*>;
using PointLightList = QList<PointLight*>;

class VOLCANO_GAME_API Scene: public Object {
    Q_OBJECT
    Q_PROPERTY(Light* ambientLight READ ambientLight WRITE setAmbientLight NOTIFY ambientLightChanged)
    Q_PROPERTY(QQmlListProperty<Actor> actors READ qmlActorList)
    Q_CLASSINFO("DefaultProperty", "actors")

public:
    Scene(QObject* parent = nullptr);
    Scene(Context& context, QObject* parent = nullptr);

public:
    const ActorList& actorList(void) const;
    const EntityList& entityList() const;
    Light* ambientLight();
    void setAmbientLight(Light* p);
    const DirectionalLightList& directionalLightList() const;
    const SpotLightList& spotLightList() const;
    const PointLightList& pointLightList() const;
    void appendActor(Actor* actor);
    qsizetype actorCount(void) const;
    Actor* actorAt(qsizetype index);
    void clearActors(void);
    void replaceActor(qsizetype index, Actor* actor);
    void removeLastActor(void);
    QQmlListProperty<Actor> qmlActorList();

signals:
    void actorAdded(Actor* p);
    void actorRemoved(Actor* p);
    void ambientLightChanged(Light* p);
    void entityAdded(Entity* p);
    void entityRemoved(Entity* p);
    void directionalLightAdded(DirectionalLight* p);
    void directionalLightRemoved(DirectionalLight* p);
    void spotLightAdded(SpotLight* p);
    void spotLightRemoved(SpotLight* p);
    void pointLightAdded(PointLight* p);
    void pointLightRemoved(PointLight* p);

protected:
    void updateView() const override;
    void updateState(Duration elapsed) override;
    void handleActorAdded(Actor* actor);
    void handleActorRemoved(Actor* actor);

private:
    ActorList actorList_;
    EntityList entityList_;
    Light* ambientLight_;
    DirectionalLightList directionalLightList_;
    SpotLightList spotLightList_;
    PointLightList pointLightList_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCENE_HPP
