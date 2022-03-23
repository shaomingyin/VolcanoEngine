//
//
#ifndef VOLCANO_EDITOR_NEWPROJECTWIZARDPAGEINTRO_HPP
#define VOLCANO_EDITOR_NEWPROJECTWIZARDPAGEINTRO_HPP

#include <utils/wizardpage.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class NewProjectWizardPageIntro: public Utils::WizardPage {
    Q_OBJECT

public:
    NewProjectWizardPageIntro(QWidget *parent = nullptr);
    ~NewProjectWizardPageIntro(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_NEWPROJECTWIZARDPAGEINTRO_HPP
