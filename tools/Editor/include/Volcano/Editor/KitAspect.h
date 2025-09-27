//
//
#ifndef VOLCANO_EDITOR_KITASPECT_H
#define VOLCANO_EDITOR_KITASPECT_H

#include <QString>
#include <QLabel>

#include <utils/id.h>
#include <projectexplorer/kitaspect.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/Context.h>

VOLCANO_EDITOR_BEGIN

class KitAspect: public ProjectExplorer::KitAspect {
    Q_OBJECT

public:
    static const Utils::Id Id;

public:
    KitAspect(Context& context, ProjectExplorer::Kit* kit, const ProjectExplorer::KitAspectFactory* factory);

public:
    void addToInnerLayout(Layouting::Layout &layout) override;

private:
    Context& context_;
    QLabel label1_;
    QLabel label2_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_KITASPECT_H
