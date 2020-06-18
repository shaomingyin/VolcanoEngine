//
//
#ifndef VOLCANO_RENDERER_H
#define VOLCANO_RENDERER_H

#include <QObject>
#include <QString>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <Volcano/Common.h>
#include <Volcano/Scene.h>

VOLCANO_BEGIN

class VOLCANO_API Renderer: public Scene::Listener
{
public:
    Renderer(Scene *scene);
    ~Renderer(void) override;

public:
    void update(float elapsed) override;
    void render(void) override;
};

VOLCANO_END

#endif // VOLCANO_RENDERER_H
