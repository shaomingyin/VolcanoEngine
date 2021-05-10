//
//
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
	m_projectView.init();
	m_projectViewDocker.setWidget(&m_projectView);
	addDockWidget(Qt::LeftDockWidgetArea, &m_projectViewDocker);

	setCentralWidget(&m_mdiArea);

	m_fileMenu = menuBar()->addMenu("&File");

	m_fileNew = m_fileMenu->addAction("&New...");
	connect(m_fileNew, &QAction::triggered, this, &MainWindow::onFileNew);

	m_fileOpen = m_fileMenu->addAction("&Open...");
	connect(m_fileOpen, &QAction::triggered, this, &MainWindow::onFileClose);

	m_fileClose = m_fileMenu->addAction("&Close");

	m_fileSave = m_fileMenu->addAction("&Save");

	m_fileSave = m_fileMenu->addAction("Save &As...");

	m_fileMenu->addSeparator();

	m_fileQuit = m_fileMenu->addAction("&Quit");

	m_editMenu = menuBar()->addMenu("&Edit");

	m_editUndo = m_editMenu->addAction("&Undo");

	m_editRedo = m_editMenu->addAction("&Redo");

	m_editMenu->addSeparator();

	m_editCut = m_editMenu->addAction("Cut(&X)");

	m_editCopy = m_editMenu->addAction("&Copy");

	m_editPaste = m_editMenu->addAction("&Paste");

	m_viewMenu = menuBar()->addMenu("&View");

	m_viewProject = m_viewMenu->addAction("&Project");
	m_viewProject->setCheckable(true);
	m_viewProject->setChecked(m_projectViewDocker.isVisible());
	connect(m_viewProject, &QAction::triggered, this, &MainWindow::onViewProject);

	m_viewProperty = m_viewMenu->addAction("P&roperty");
	m_viewProperty->setCheckable(true);

	m_viewOutput = m_viewMenu->addAction("&Output");
	m_viewOutput->setCheckable(true);

	m_toolMenu = menuBar()->addMenu("&Tool");

	m_toolOptions = m_toolMenu->addAction("&Options...");

	m_helpMenu = menuBar()->addMenu("&Help");

	m_helpAbout = m_helpMenu->addAction("&About...");
	connect(m_helpAbout, &QAction::triggered, this, &MainWindow::onHelpAbout);

	m_helpAboutQt = m_helpMenu->addAction("About Qt...");
	connect(m_helpAboutQt, &QAction::triggered, this, &MainWindow::onHelpAboutQt);

	return true;
}

void MainWindow::onFileNew(bool v)
{
}

void MainWindow::onFileOpen(bool v)
{
}

void MainWindow::onFileClose(bool v)
{
}

void MainWindow::onFileSave(bool v)
{
}

void MainWindow::onFileSaveAs(bool v)
{
}

void MainWindow::onFileQuit(bool v)
{
}

void MainWindow::onEditUndo(bool v)
{
}

void MainWindow::onEditRedo(bool v)
{
}

void MainWindow::onEditCut(bool v)
{
}

void MainWindow::onEditCopy(bool v)
{
}

void MainWindow::onEditPaste(bool v)
{
}

void MainWindow::onViewProject(bool v)
{
	m_projectViewDocker.setVisible(v);
}

void MainWindow::onViewProperty(bool v)
{
}

void MainWindow::onViewOutput(bool v)
{
}

void MainWindow::onToolOptions(bool v)
{
}

void MainWindow::onHelpAbout(bool v)
{
	QMessageBox::about(this, "About", "About Volcano Editor");
}

void MainWindow::onHelpAboutQt(bool v)
{
	QMessageBox::aboutQt(this);
}

VOLCANO_EDITOR_END
