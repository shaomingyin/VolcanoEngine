//
//
#include <memory>

#include <QMessageBox>

#include <Volcano/Editor/MainWindow.hpp>

VOLCANO_EDITOR_BEGIN

MainWindow::MainWindow(void)
{
}

MainWindow::~MainWindow(void)
{
}

bool MainWindow::init(void)
{
    if (!MainWindowBase::init())
        return false;

    return true;
}

Project *MainWindow::newProject(const QString &name, const QString &dirName)
{
    auto project = std::make_unique<Project>(this);
    if (!project || !project->init(dirName, name)) {
        QMessageBox::critical(this, "Failed to initialize project", "TODO");
        return nullptr;
    }

    return project.release();
}

VOLCANO_EDITOR_END
