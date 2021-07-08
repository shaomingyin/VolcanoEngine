//
//
#include <QSettings>
#include <QMenuBar>
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
    QSettings settings;
    restoreGeometry(settings.value("mainWindow/geometry").toByteArray());
    restoreState(settings.value("mainWindow/state").toByteArray());

    setCentralWidget(&m_mdiArea);

    m_worldViewDocker.setWindowTitle("World");
    m_worldViewDocker.setWidget(&m_worldView);
    addDockWidget(Qt::LeftDockWidgetArea, &m_worldViewDocker);
    restoreDockWidget(&m_worldViewDocker);

    m_propertyViewDocker.setWindowTitle("Property");
    m_propertyViewDocker.setWidget(&m_propertyView);
    addDockWidget(Qt::RightDockWidgetArea, &m_propertyViewDocker);
    restoreDockWidget(&m_propertyViewDocker);

    m_resourceViewDocker.setWindowTitle("Resource");
    m_resourceViewDocker.setWidget(&m_resourceView);
    addDockWidget(Qt::BottomDockWidgetArea, &m_resourceViewDocker);
    restoreDockWidget(&m_resourceViewDocker);

    m_outputViewDocker.setWindowTitle("Output");
    m_outputViewDocker.setWidget(&m_outputView);
    addDockWidget(Qt::BottomDockWidgetArea, &m_outputViewDocker);
    restoreDockWidget(&m_outputViewDocker);

    initMenuBar();
    initToolBar();

    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("mainWindow/geometry", saveGeometry());
    settings.setValue("mainWindow/state", saveState());

    QMainWindow::closeEvent(event);
}

void MainWindow::initMenuBar(void)
{
    // file

    m_fileMenu = menuBar()->addMenu("File");

    m_fileNew = m_fileMenu->addAction("New...");
    connect(m_fileNew, &QAction::triggered, this, &MainWindow::onFileNew);

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

    m_viewResource = m_viewMenu->addAction("Resource");
    m_viewResource->setCheckable(true);
    m_viewResource->setChecked(m_resourceViewDocker.isVisible());

    m_viewOutput = m_viewMenu->addAction("Output");

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

void MainWindow::onFileNew(void)
{

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

}

void MainWindow::onViewResource(bool checked)
{

}

void MainWindow::onViewOutput(bool checked)
{

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

VOLCANO_EDITOR_END
