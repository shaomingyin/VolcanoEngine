//
//
#ifndef VOLCANO_EDITOR_MAINWINDOW_HPP
#define VOLCANO_EDITOR_MAINWINDOW_HPP

#include <QMap>
#include <QSharedPointer>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QDockWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMainWindow>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/ProjectView.hpp>
#include <Volcano/Editor/WorldView.hpp>

VOLCANO_EDITOR_BEGIN

class MainWindow: public QMainWindow {
	Q_OBJECT

public:
	MainWindow(void);
	~MainWindow(void) override;

public:
	bool init(void);

private slots:
	void onFileNew(bool v);
	void onFileOpen(bool v);
	void onFileClose(bool v);
	void onFileSave(bool v);
	void onFileSaveAs(bool v);
	void onEditPaste(bool v);
	void onFileQuit(bool v);
	void onEditCopy(bool v);
	void onEditCut(bool v);
	void onEditRedo(bool v);
	void onEditUndo(bool v);
	void onViewOutput(bool v);
	void onViewProperty(bool v);
	void onViewProject(bool v);
	void onToolOptions(bool v);
	void onHelpAbout(bool v);
	void onHelpAboutQt(bool v);

private:
	QMenu *m_fileMenu;
	QMenu *m_editMenu;
	QMenu *m_viewMenu;
	QMenu *m_toolMenu;
	QMenu *m_helpMenu;

private:
	QAction *m_fileNew;
	QAction *m_fileOpen;
	QAction *m_fileClose;
	QAction *m_fileSave;
	QAction *m_fileSaveAs;
	QAction *m_fileQuit;
	QAction *m_editUndo;
	QAction *m_editRedo;
	QAction *m_editCut;
	QAction *m_editCopy;
	QAction *m_editPaste;
	QAction *m_viewProject;
	QAction *m_viewProperty;
	QAction *m_viewOutput;
	QAction *m_toolOptions;
	QAction *m_helpAbout;
	QAction *m_helpAboutQt;

private:
	QDockWidget m_projectViewDocker;
	ProjectView m_projectView;

	QMdiArea m_mdiArea;

private:
	using WorldViewPtr = QSharedPointer<WorldView>;
	using WorldViews = QMap<QMdiSubWindow *, WorldViewPtr>;

	WorldViews m_worldViews;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_MAINWINDOW_HPP
