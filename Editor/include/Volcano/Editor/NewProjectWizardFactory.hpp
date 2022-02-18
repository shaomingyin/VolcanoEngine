//
//
#ifndef VOLCANO_EDITOR_NEWPROJECTWIZARDFACTORY_HPP
#define VOLCANO_EDITOR_NEWPROJECTWIZARDFACTORY_HPP

#include <utils/id.h>
#include <utils/wizard.h>
#include <coreplugin/iwizardfactory.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class NewProjectWizardFactory: public Core::IWizardFactory {
    Q_OBJECT

public:
    NewProjectWizardFactory(void);
    ~NewProjectWizardFactory(void) override;

protected:
    Utils::Wizard *runWizardImpl(const Utils::FilePath &path, QWidget *parent,
        Utils::Id platform, const QVariantMap &variables, bool showWizard) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_NEWPROJECTWIZARDFACTORY_HPP
