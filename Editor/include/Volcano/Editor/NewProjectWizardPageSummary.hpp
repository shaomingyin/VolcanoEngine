//
//
#ifndef VOLCANO_EDITOR_NEWPROJECTWIZARDPAGESUMMARY_HPP
#define VOLCANO_EDITOR_NEWPROJECTWIZARDPAGESUMMARY_HPP

#include <utils/wizardpage.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class NewProjectWizardPageSummary: public Utils::WizardPage {
    Q_OBJECT

public:
    NewProjectWizardPageSummary(QWidget *parent = nullptr);
    ~NewProjectWizardPageSummary(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_NEWPROJECTWIZARDPAGESUMMARY_HPP
