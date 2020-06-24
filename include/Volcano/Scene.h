//
//
#ifndef VOLCANO_SCENE_H
#define VOLCANO_SCENE_H

#include <QVector>
#include <QObject>
#include <QVector3D>
#include <QScopedPointer>

#include <Volcano/Common.h>
#include <Volcano/Node.h>
#include <Volcano/Entity.h>

VOLCANO_BEGIN

class VOLCANO_API Scene: public Node
{
    Q_OBJECT

public:
    Q_INVOKABLE Scene(QObject *parent = nullptr);
    ~Scene(void) override;

public:
    virtual void update(float elapsed);
};

VOLCANO_END

#endif // VOLCANO_SCENE_H
