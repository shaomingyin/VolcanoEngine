//
//
#ifndef VOLCANO_SYSTEM_POTENTIALVISIBLESETBUILDER_H
#define VOLCANO_SYSTEM_POTENTIALVISIBLESETBUILDER_H

#include <QObject>

#include <Volcano/World/Scene.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class PotentialVisibleSetBuilder: public QObject {
    Q_OBJECT

public:
    PotentialVisibleSetBuilder(const World::Scene& scene, QObject* parent = nullptr);

public:
    const World::Scene& scene() const {
        return scene_;
    }

    void build(Graphics::View& view) const;

private:
    const World::Scene& scene_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_POTENTIALVISIBLESETBUILDER_H
