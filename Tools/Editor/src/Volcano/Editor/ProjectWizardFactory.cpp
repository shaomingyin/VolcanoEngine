//
//
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <projectexplorer/projectexplorericons.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <cmakeprojectmanager/cmakeprojectconstants.h>

#include <Volcano/Editor/ProjectWizard.hpp>
#include <Volcano/Editor/ProjectWizardFactory.hpp>

VOLCANO_EDITOR_BEGIN

ProjectWizardFactory::ProjectWizardFactory()
    /*: icon_("qrc:/icons/logo.png")*/ {
    //setIcon(icon_);
    setSupportedProjectTypes({ "volcano" });
    setDisplayName("Volcano Project");
    setId("V.VolcanoProjectFile");
    setDescription("XXX.");
    setCategory(ProjectExplorer::Constants::QT_PROJECT_WIZARD_CATEGORY);
    setDisplayCategory(ProjectExplorer::Constants::QT_PROJECT_WIZARD_CATEGORY_DISPLAY);
    setFlags(IWizardFactory::PlatformIndependent);
}

Core::BaseFileWizard *ProjectWizardFactory::create(QWidget *parent, const Core::WizardDialogParameters &parameters) const {
#if 0
    return new class ProjectWizard(this, parent);
#else
    return nullptr;
#endif
}

Core::GeneratedFiles ProjectWizardFactory::generateFiles(const QWizard *wizard, QString *error_message) const {
    const class ProjectWizard* project_wizard = qobject_cast<const class ProjectWizard*>(wizard);
#if 0
    QJsonObject scene;
    scene["graivty"] = QJsonArray({0.0f, -9.8f, 0.0f});

    QJsonObject camera;
    camera["fov"] = 90;

    QJsonObject world;
    world["camera"] = camera;
    world["scenes"] = QJsonArray({ scene });

    QJsonObject project;
    project["version"] = "0.1.0";
    project["engineVersion"] = VOLCANO_VERSION_STR;
    project["name"] = project_wizard->projectName();
    project["world"] = world;

    QJsonDocument project_doc;
    project_doc.setObject(project);

    auto project_filepath = project_wizard->filePath().pathAppended(project_wizard->projectName() + ".vpf");
    Core::GeneratedFile project_file(project_filepath);
    project_file.setContents(QString::fromUtf8(project_doc.toJson()));
    project_file.setAttributes(Core::GeneratedFile::OpenProjectAttribute);

    return { project_file };
#else
    return {};
#endif
}

VOLCANO_EDITOR_END
