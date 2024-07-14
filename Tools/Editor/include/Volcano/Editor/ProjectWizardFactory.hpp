//
//
#ifndef VOLCANO_EDITOR_PROJECTWIZARDFACTORY_HPP
#define VOLCANO_EDITOR_PROJECTWIZARDFACTORY_HPP

#include <QIcon>
#include <QWidget>

#include <coreplugin/basefilewizardfactory.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class ProjectWizardFactory: public Core::BaseFileWizardFactory {
    Q_OBJECT

public:
    ProjectWizardFactory();
    ~ProjectWizardFactory() override = default;

public:
    Core::BaseFileWizard *create(QWidget *parent, const Core::WizardDialogParameters &parameters) const override;
    Core::GeneratedFiles generateFiles(const QWizard *wizard, QString *error_message) const override;

private:
    //QIcon icon_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECTWIZARDFACTORY_HPP
