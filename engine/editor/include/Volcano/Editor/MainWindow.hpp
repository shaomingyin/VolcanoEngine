//
//
#ifndef VOLCANO_EDITOR_MAINWINDOW_HPP
#define VOLCANO_EDITOR_MAINWINDOW_HPP

#include <QCloseEvent>
#include <QAction>
#include <QMenu>
#include <QMdiArea>
#include <QDockWidget>
#include <QMainWindow>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/Project.hpp>
#include <Volcano/Editor/CameraView.hpp>
#include <Volcano/Editor/OutputView.hpp>
#include <Volcano/Editor/PropertyView.hpp>
#include <Volcano/Editor/ResourceView.hpp>
#include <Volcano/Editor/WorldView.hpp>

VOLCANO_EDITOR_BEGIN

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(void);
    ~MainWindow(void) override;

public:
    bool init(void);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void initMenuBar(void);
    void initToolBar(void);

private slots:
    void onFileNew(void);
    void onFileOpen(void);
    void onFileClose(void);
    void onFileSave(void);
    void onFileSaveAs(void);
    void onFileQuit(void);
    void onEditCut(void);
    void onEditCopy(void);
    void onEditPaste(void);
    void onViewWorld(bool checked);
    void onViewResource(bool checked);
    void onViewOutput(bool checked);
    void onProjectImport(void);
    void onProjectExport(void);
    void onProjectBuild(void);
    void onProjectClean(void);
    void onToolsPreferences(void);
    void onHelpAbout(void);
    void onHelpAboutQt(void);

private:
    Project *m_project;

    QMenu *m_fileMenu;
    QAction *m_fileNew;
    QAction *m_fileOpen;
    QAction *m_fileClose;
    QAction *m_fileSave;
    QAction *m_fileSaveAs;
    QAction *m_fileQuit;

    QMenu *m_editMenu;
    QAction *m_editCut;
    QAction *m_editCopy;
    QAction *m_editPaste;

    QMenu *m_viewMenu;
    QAction *m_viewWorld;
    QAction *m_viewResource;
    QAction *m_viewOutput;

    QMenu *m_projectMenu;
    QAction *m_projectImport;
    QAction *m_projectExport;
    QAction *m_projectBuild;
    QAction *m_projectClean;

    QMenu *m_toolsMenu;
    QAction *m_toolsPreferences;

    QMenu *m_helpMenu;
    QAction *m_helpAbout;
    QAction *m_helpAboutQt;

    QMdiArea m_mdiArea;

    QDockWidget m_worldViewDocker;
    WorldView m_worldView;

    QDockWidget m_propertyViewDocker;
    PropertyView m_propertyView;

    QDockWidget m_resourceViewDocker;
    ResourceView m_resourceView;

    QDockWidget m_outputViewDocker;
    OutputView m_outputView;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_MAINWINDOW_HPP
