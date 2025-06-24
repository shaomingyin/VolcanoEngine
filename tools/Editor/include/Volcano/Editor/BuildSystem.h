//
//
#ifndef VOLCANO_EDITOR_BUILDSYSTEM_H
#define VOLCANO_EDITOR_BUILDSYSTEM_H

#include <memory>

#include <QFuture>

#include <projectexplorer/target.h>
#include <projectexplorer/buildsystem.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class BuildSystem: public ProjectExplorer::BuildSystem {
    Q_OBJECT

public:
    explicit BuildSystem(ProjectExplorer::Target *target);
    explicit BuildSystem(ProjectExplorer::BuildConfiguration *bc);

public:
    QString name() const override;
    void triggerParsing() override;

private:
    void parse(QPromise<void>& promise);

private:
    QFuture<void> parsing_future_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_BUILDSYSTEM_H
