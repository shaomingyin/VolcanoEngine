//
//
#ifndef VOLCANO_EDITOR_NEWPROJECTWIZARD_HPP
#define VOLCANO_EDITOR_NEWPROJECTWIZARD_HPP

#include <QWizard>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/NewProjectInfoPage.hpp>

VOLCANO_EDITOR_BEGIN

class NewProjectWizard: public QWizard {
    Q_OBJECT

public:
    NewProjectWizard(QWidget *parent = nullptr);
    ~NewProjectWizard(void) override;

private:
    NewProjectInfoPage m_newProjectInfoPage;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_NEWPROJECTWIZARD_HPP
