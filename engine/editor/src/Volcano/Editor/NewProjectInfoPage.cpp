//
//
#include <QFileDialog>

#include <Volcano/Editor/NewProjectInfoPage.hpp>

VOLCANO_EDITOR_BEGIN

NewProjectInfoPage::NewProjectInfoPage(QWidget *parent):
    QWizardPage(parent)
{
    setTitle("Project Information");
}

NewProjectInfoPage::~NewProjectInfoPage(void)
{
}

void NewProjectInfoPage::initializePage(void)
{
    m_name.setText("NewVolcanoProject1");
    m_name.selectAll();
    m_layout.addRow("Name:", &m_name);
    registerField("name", &m_name);

    m_description.setText("This is description...");
    m_layout.addRow("Description:", &m_description);
    registerField("description", &m_description);

    m_pathLayout.addWidget(&m_path);
    registerField("path", &m_path);

    m_pathBrowser.setText("...");
    m_pathLayout.addWidget(&m_pathBrowser);
    connect(&m_pathBrowser, &QPushButton::clicked, [this] {
        m_path.setText(QFileDialog::getExistingDirectory(this, "Project Path"));
    });

    m_layout.addRow("Path:", &m_pathLayout);

    setLayout(&m_layout);
}

VOLCANO_EDITOR_END
