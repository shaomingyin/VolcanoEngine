//
//
#include <QSettings>
#include <QMenuBar>
#include <QMessageBox>
#include <QDir>

#include <Volcano/Editor/NewProjectWizard.hpp>
#include <Volcano/Editor/MainWindow.hpp>

VOLCANO_EDITOR_BEGIN

MainWindow::MainWindow(void):
    m_project(nullptr)
{
}

MainWindow::~MainWindow(void)
{
}

bool MainWindow::init(void)
{
    QSettings settings;
    restoreGeometry(settings.value("mainWindow/geometry").toByteArray());
    restoreState(settings.value("mainWindow/state").toByteArray());

    setCentralWidget(&m_viewport);

    m_worldView = new WorldView(this);
    m_worldViewDocker.setWidget(m_worldView);
    m_worldViewDocker.setWindowTitle("World");
    addDockWidget(Qt::LeftDockWidgetArea, &m_worldViewDocker);
    restoreDockWidget(&m_worldViewDocker);
    connect(m_worldView, &WorldView::doubleClicked, this, &MainWindow::onWorldViewItemDoubleClicked);

    m_propertyView = new PropertyView(this);
    m_propertyViewDocker.setWidget(m_propertyView);
    m_propertyViewDocker.setWindowTitle("Property");
    addDockWidget(Qt::RightDockWidgetArea, &m_propertyViewDocker);
    restoreDockWidget(&m_propertyViewDocker);

    m_resourceView = new ResourceView(this);
    m_resourceViewDocker.setWidget(m_resourceView);
    m_resourceViewDocker.setWindowTitle("Resource");
    addDockWidget(Qt::BottomDockWidgetArea, &m_resourceViewDocker);
    restoreDockWidget(&m_resourceViewDocker);

    m_outputView = new OutputView(this);
    m_outputViewDocker.setWidget(m_outputView);
    m_outputViewDocker.setWindowTitle("Output");
    addDockWidget(Qt::BottomDockWidgetArea, &m_outputViewDocker);
    restoreDockWidget(&m_outputViewDocker);

    initMenuBar();
    initToolBar();

    return true;
}

Project *MainWindow::project(void)
{
    return m_project;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("mainWindow/geometry", saveGeometry());
    settings.setValue("mainWindow/state", saveState());

    QMainWindow::closeEvent(event);
}

void MainWindow::setProject(Project *project)
{
    if (m_project == project)
        return;

    m_worldView->setModel(&project->worldModel());

    auto &resModel = project->resourcesModel();
    m_resourceView->setModel(&resModel);
    m_resourceView->setRootIndex(resModel.index(project->resourcesDir().path()));

    m_project = project;
    emit projectChanged(project);
}

void MainWindow::initMenuBar(void)
{
    // file

    m_fileMenu = menuBar()->addMenu("File");

    m_fileNewMenu = m_fileMenu->addMenu("New");

    m_fileNewProject = m_fileNewMenu->addAction("Project...");
    connect(m_fileNewProject, &QAction::triggered, this, &MainWindow::onFileNewProject);

    m_fileOpen = m_fileMenu->addAction("Open...");
    connect(m_fileOpen, &QAction::triggered, this, &MainWindow::onFileOpen);

    m_fileClose = m_fileMenu->addAction("Close");
    connect(m_fileClose, &QAction::triggered, this, &MainWindow::onFileClose);

    m_fileSave = m_fileMenu->addAction("Save");
    connect(m_fileSave, &QAction::triggered, this, &MainWindow::onFileSave);

    m_fileSaveAs = m_fileMenu->addAction("Save As...");
    connect(m_fileSaveAs, &QAction::triggered, this, &MainWindow::onFileSaveAs);

    m_fileMenu->addSeparator();

    m_fileQuit = m_fileMenu->addAction("Quit");
    connect(m_fileQuit, &QAction::triggered, this, &MainWindow::onFileQuit);

    // edit

    m_editMenu = menuBar()->addMenu("Edit");

    m_editCut = m_editMenu->addAction("Cut");
    connect(m_editCut, &QAction::triggered, this, &MainWindow::onEditCut);

    m_editCopy = m_editMenu->addAction("Copy");
    connect(m_editCopy, &QAction::triggered, this, &MainWindow::onEditCopy);

    m_editPaste = m_editMenu->addAction("Paste");
    connect(m_editPaste, &QAction::triggered, this, &MainWindow::onEditPaste);

    // view


    m_viewMenu = menuBar()->addMenu("View");

    m_viewWorld = m_viewMenu->addAction("World");
    m_viewWorld->setCheckable(true);
    m_viewWorld->setChecked(m_worldViewDocker.isVisible());
    connect(m_viewWorld, &QAction::toggled, this, &MainWindow::onViewWorld);

    m_viewProperty = m_viewMenu->addAction("Property");
    m_viewProperty->setCheckable(true);
    m_viewProperty->setChecked(m_propertyViewDocker.isVisible());
    connect(m_viewProperty, &QAction::toggled, this, &MainWindow::onViewProperty);

    m_viewResource = m_viewMenu->addAction("Resource");
    m_viewResource->setCheckable(true);
    m_viewResource->setChecked(m_resourceViewDocker.isVisible());
    connect(m_viewResource, &QAction::toggled, this, &MainWindow::onViewResource);

    m_viewOutput = m_viewMenu->addAction("Output");
    m_viewOutput->setCheckable(true);
    m_viewOutput->setChecked(m_outputViewDocker.isVisible());
    connect(m_viewOutput, &QAction::toggled, this, &MainWindow::onViewOutput);

    connect(m_viewMenu, &QMenu::aboutToShow, [this] {
        m_viewWorld->setChecked(m_worldViewDocker.isVisible());
        m_viewProperty->setChecked(m_propertyViewDocker.isVisible());
        m_viewResource->setChecked(m_resourceViewDocker.isVisible());
        m_viewOutput->setChecked(m_outputViewDocker.isVisible());
    });

    // project

    m_projectMenu = menuBar()->addMenu("Project");

    m_projectImport = m_projectMenu->addAction("Import...");

    m_projectExport = m_projectMenu->addAction("Export...");

    m_projectBuild = m_projectMenu->addAction("Build");

    m_projectClean = m_projectMenu->addAction("Clean");

    // tools

    m_toolsMenu = menuBar()->addMenu("Tools");

    m_toolsPreferences = m_toolsMenu->addAction("Preferences...");

    // help

    m_helpMenu = menuBar()->addMenu("Help");

    m_helpAbout = m_helpMenu->addAction("About...");

    m_helpAboutQt = m_helpMenu->addAction("About Qt...");
    connect(m_helpAboutQt, &QAction::triggered, this, &MainWindow::onHelpAboutQt);
}

void MainWindow::initToolBar(void)
{
}

void MainWindow::onFileNewProject(void)
{
    NewProjectWizard newProjectWizard(this);
    newProjectWizard.exec();
    if (newProjectWizard.result() != QDialog::Accepted)
        return;

    if (m_project != nullptr) {
        m_worldView->setModel(nullptr);
        m_resourceView->setModel(nullptr);
        m_propertyView->setModel(nullptr);
        delete m_project;
        m_project = nullptr;
    }

    auto name = newProjectWizard.field("name").toString();
    //auto description = newProjectWizard.field("description").toString();
    auto path = newProjectWizard.field("path").toString();

    QDir dir(path);
    if (!dir.isEmpty()) {
        QMessageBox::critical(this, "Failed to create project",
            QString("The directory '%1' is not empty.").arg(path));
        return;
    }

    auto project = std::make_unique<Project>(this);
    if (!project || !project->init(path, name)) {
        QMessageBox::critical(this, "Failed to initialize project", "TODO");
        return;
    }

    setProject(project.release());
}

void MainWindow::onFileOpen(void)
{

}

void MainWindow::onFileClose(void)
{

}

void MainWindow::onFileSave(void)
{

}

void MainWindow::onFileSaveAs(void)
{

}

void MainWindow::onFileQuit(void)
{

}

void MainWindow::onEditCut(void)
{

}

void MainWindow::onEditCopy(void)
{

}

void MainWindow::onEditPaste(void)
{

}

void MainWindow::onViewWorld(bool checked)
{
    m_worldViewDocker.setVisible(checked);
}

void MainWindow::onViewProperty(bool checked)
{
    m_propertyViewDocker.setVisible(checked);
}

void MainWindow::onViewResource(bool checked)
{
    m_resourceViewDocker.setVisible(checked);
}

void MainWindow::onViewOutput(bool checked)
{
    m_outputViewDocker.setVisible(checked);
}

void MainWindow::onProjectImport(void)
{

}

void MainWindow::onProjectExport(void)
{

}

void MainWindow::onProjectBuild(void)
{

}

void MainWindow::onProjectClean(void)
{

}

void MainWindow::onToolsPreferences(void)
{

}

void MainWindow::onHelpAbout(void)
{
}

void MainWindow::onHelpAboutQt(void)
{
    QMessageBox::aboutQt(this);
}

void MainWindow::onWorldViewItemDoubleClicked(const QModelIndex &index)
{
    if (index.parent().isValid())
        return;
}

VOLCANO_EDITOR_END
