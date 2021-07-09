//
//
#ifndef VOLCANO_EDITOR_NEWPROJECTINFOPAGE_HPP
#define VOLCANO_EDITOR_NEWPROJECTINFOPAGE_HPP

#include <QWizardPage>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class NewProjectInfoPage: public QWizardPage {
    Q_OBJECT

public:
    NewProjectInfoPage(QWidget *parent = nullptr);
    ~NewProjectInfoPage(void) override;

public:
    void initializePage(void) override;

private:
    QFormLayout m_layout;
    QLineEdit m_name;
    QLineEdit m_description;
    QHBoxLayout m_pathLayout;
    QLineEdit m_path;
    QPushButton m_pathBrowser;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_NEWPROJECTINFOPAGE_HPP
