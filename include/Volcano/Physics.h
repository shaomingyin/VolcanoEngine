//
//
#ifndef VOLCANO_PHYSICS_H
#define VOLCANO_PHYSICS_H

#include <QObject>
#include <QString>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <Volcano/Common.h>
#include <Volcano/Scene.h>

VOLCANO_BEGIN

class VOLCANO_API Physics: public Scene::Listener
{
public:
    Physics(Scene *scene);
    ~Physics(void) override;

public:
    void update(float elapsed) override;
};

VOLCANO_END

#endif // VOLCANO_PHYSICS_H
