//
//
#ifndef VOLCANO_EDITOR_NEWPROJECTWIZARD_HPP
#define VOLCANO_EDITOR_NEWPROJECTWIZARD_HPP

#include <utils/wizard.h>
#include <utils/filepath.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class NewProjectWizard: public Utils::Wizard {
    Q_OBJECT

public:
    enum {
        PageIntro = 0,
        PageBasic,
        PageSummary
    };

public:
    NewProjectWizard(const Utils::FilePath &path, QWidget *parent = nullptr);
    ~NewProjectWizard(void) override;

private:
    Utils::FilePath m_path;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_NEWPROJECTWIZARD_HPP
