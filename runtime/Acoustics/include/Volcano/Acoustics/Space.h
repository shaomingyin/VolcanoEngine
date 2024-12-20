//
//
#ifndef VOLCANO_ACOUSTICS_SPACE_H
#define VOLCANO_ACOUSTICS_SPACE_H

#include <QObject>

#include <Volcano/World/Listener.h>
#include <Volcano/Acoustics/Common.h>

VOLCANO_ACOUSTICS_BEGIN

class Space: public World::Listener {
    Q_OBJECT

public:
    Space(World::Scene& scene, QObject* parent = nullptr);

public:
    virtual void update(Duration elapsed);
    virtual void render();

protected:
    void onComponentAdded(World::Entity* entity, World::Component* component) override;
    void onComponentRemoved(World::Entity* entity, World::Component* component) override;
};

VOLCANO_ACOUSTICS_END

#endif // VOLCANO_ACOUSTICS_SPACE_H
