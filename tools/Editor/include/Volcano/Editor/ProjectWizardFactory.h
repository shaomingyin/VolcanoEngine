//
//
#ifndef VOLCANO_EDITOR_PROJECTWIZARDFACTORY_H
#define VOLCANO_EDITOR_PROJECTWIZARDFACTORY_H

#include <coreplugin/basefilewizardfactory.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class ProjectWizardFactory: public Core::BaseFileWizardFactory {
    Q_OBJECT

public:
    ProjectWizardFactory();

public:
    Core::BaseFileWizard* create(QWidget* parent, const Core::WizardDialogParameters& parameters) const override;
    Core::GeneratedFiles generateFiles(const QWizard* wizard, QString* error_message) const override;
    bool writeFiles(const Core::GeneratedFiles& files, QString* error_message) const override;
    bool postGenerateFiles(const QWizard* wizard, const Core::GeneratedFiles& gen_files, QString* error_message) const override;

};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECTWIZARDFACTORY_H
