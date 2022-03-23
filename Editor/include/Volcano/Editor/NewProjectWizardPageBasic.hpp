//
//
#ifndef VOLCANO_EDITOR_NEWPROJECTWIZARDPAGEBASIC_HPP
#define VOLCANO_EDITOR_NEWPROJECTWIZARDPAGEBASIC_HPP

#include <utils/wizardpage.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class NewProjectWizardPageBasic: public Utils::WizardPage {
    Q_OBJECT

public:
    NewProjectWizardPageBasic(QWidget *parent = nullptr);
    ~NewProjectWizardPageBasic(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_NEWPROJECTWIZARDPAGEBASIC_HPP
