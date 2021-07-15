//
//
#include <QSettings>
#include <QMenuBar>
#include <QMessageBox>
#include <QDir>

#include <Volcano/Editor/NewProjectWizard.hpp>
#include <Volcano/Editor/MainWindowBase.hpp>

VOLCANO_EDITOR_BEGIN

MainWindowBase::MainWindowBase(void):
    m_project(nullptr)
{
}

MainWindowBase::~MainWindowBase(void)
{
}

bool MainWindowBase::init(void)
{
    QSettings settings;
    restoreGeometry(settings.value("mainWindow/geometry").toByteArray());
    restoreState(settings.value("mainWindow/state").toByteArray());

    setCentralWidget(&m_mdiArea);

    m_worldView = new WorldView(this);
    m_worldViewDocker.setWidget(m_worldView);
    m_worldViewDocker.setWindowTitle("World");
    addDockWidget(Qt::LeftDockWidgetArea, &m_worldViewDocker);
    restoreDockWidget(&m_worldViewDocker);
    connect(m_worldView, &WorldView::doubleClicked, this, &MainWindowBase::onWorldViewItemDoubleClicked);

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

Project *MainWindowBase::project(void)
{
    return m_project;
}

void MainWindowBase::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("mainWindow/geometry", saveGeometry());
    settings.setValue("mainWindow/state", saveState());

    QMainWindow::closeEvent(event);
}

void MainWindowBase::setProject(Project *project)
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

void MainWindowBase::initMenuBar(void)
{
    // file

    m_fileMenu = menuBar()->addMenu("File");

    m_fileNewMenu = m_fileMenu->addMenu("New");

    m_fileNewProject = m_fileNewMenu->addAction("Project...");
    connect(m_fileNewProject, &QAction::triggered, this, &MainWindowBase::onFileNewProject);

    m_fileOpen = m_fileMenu->addAction("Open...");
    connect(m_fileOpen, &QAction::triggered, this, &MainWindowBase::onFileOpen);

    m_fileClose = m_fileMenu->addAction("Close");
    connect(m_fileClose, &QAction::triggered, this, &MainWindowBase::onFileClose);

    m_fileSave = m_fileMenu->addAction("Save");
    connect(m_fileSave, &QAction::triggered, this, &MainWindowBase::onFileSave);

    m_fileSaveAs = m_fileMenu->addAction("Save As...");
    connect(m_fileSaveAs, &QAction::triggered, this, &MainWindowBase::onFileSaveAs);

    m_fileMenu->addSeparator();

    m_fileQuit = m_fileMenu->addAction("Quit");
    connect(m_fileQuit, &QAction::triggered, this, &MainWindowBase::onFileQuit);

    // edit

    m_editMenu = menuBar()->addMenu("Edit");

    m_editCut = m_editMenu->addAction("Cut");
    connect(m_editCut, &QAction::triggered, this, &MainWindowBase::onEditCut);

    m_editCopy = m_editMenu->addAction("Copy");
    connect(m_editCopy, &QAction::triggered, this, &MainWindowBase::onEditCopy);

    m_editPaste = m_editMenu->addAction("Paste");
    connect(m_editPaste, &QAction::triggered, this, &MainWindowBase::onEditPaste);

    // view


    m_viewMenu = menuBar()->addMenu("View");

    m_viewWorld = m_viewMenu->addAction("World");
    m_viewWorld->setCheckable(true);
    m_viewWorld->setChecked(m_worldViewDocker.isVisible());
    connect(m_viewWorld, &QAction::toggled, this, &MainWindowBase::onViewWorld);

    m_viewProperty = m_viewMenu->addAction("Property");
    m_viewProperty->setCheckable(true);
    m_viewProperty->setChecked(m_propertyViewDocker.isVisible());
    connect(m_viewProperty, &QAction::toggled, this, &MainWindowBase::onViewProperty);

    m_viewResource = m_viewMenu->addAction("Resource");
    m_viewResource->setCheckable(true);
    m_viewResource->setChecked(m_resourceViewDocker.isVisible());
    connect(m_viewResource, &QAction::toggled, this, &MainWindowBase::onViewResource);

    m_viewOutput = m_viewMenu->addAction("Output");
    m_viewOutput->setCheckable(true);
    m_viewOutput->setChecked(m_outputViewDocker.isVisible());
    connect(m_viewOutput, &QAction::toggled, this, &MainWindowBase::onViewOutput);

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
    connect(m_helpAboutQt, &QAction::triggered, this, &MainWindowBase::onHelpAboutQt);
}

void MainWindowBase::initToolBar(void)
{
}

void MainWindowBase::onFileNewProject(void)
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

    // Todo
}

void MainWindowBase::onFileOpen(void)
{

}

void MainWindowBase::onFileClose(void)
{

}

void MainWindowBase::onFileSave(void)
{

}

void MainWindowBase::onFileSaveAs(void)
{

}

void MainWindowBase::onFileQuit(void)
{

}

void MainWindowBase::onEditCut(void)
{

}

void MainWindowBase::onEditCopy(void)
{

}

void MainWindowBase::onEditPaste(void)
{

}

void MainWindowBase::onViewWorld(bool checked)
{
    m_worldViewDocker.setVisible(checked);
}

void MainWindowBase::onViewProperty(bool checked)
{
    m_propertyViewDocker.setVisible(checked);
}

void MainWindowBase::onViewResource(bool checked)
{
    m_resourceViewDocker.setVisible(checked);
}

void MainWindowBase::onViewOutput(bool checked)
{
    m_outputViewDocker.setVisible(checked);
}

void MainWindowBase::onProjectImport(void)
{

}

void MainWindowBase::onProjectExport(void)
{

}

void MainWindowBase::onProjectBuild(void)
{

}

void MainWindowBase::onProjectClean(void)
{

}

void MainWindowBase::onToolsPreferences(void)
{

}

void MainWindowBase::onHelpAbout(void)
{

}

void MainWindowBase::onHelpAboutQt(void)
{
    QMessageBox::aboutQt(this);
}

void MainWindowBase::onWorldViewItemDoubleClicked(const QModelIndex &index)
{
    if (index.parent().isValid())
        return;

    // TODO
}

VOLCANO_EDITOR_END
