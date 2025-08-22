//
//
#ifndef VOLCANO_EDITOR_PROJECTWIZARD_H
#define VOLCANO_EDITOR_PROJECTWIZARD_H

#include <coreplugin/basefilewizard.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class ProjectWizard: public Core::BaseFileWizard {
    Q_OBJECT

public:
    ProjectWizard(const Core::BaseFileWizardFactory* factory, const QVariantMap& extra_values, QWidget* parent = nullptr);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECTWIZARD_H
