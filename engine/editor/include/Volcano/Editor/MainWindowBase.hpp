//
//
#ifndef VOLCANO_EDITOR_MAINWINDOWBASE_HPP
#define VOLCANO_EDITOR_MAINWINDOWBASE_HPP

#include <QString>
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

class MainWindowBase: public QMainWindow {
    Q_OBJECT

public:
    MainWindowBase(void);
    ~MainWindowBase(void) override;

public:
    virtual bool init(void);
    Project *project(void);

signals:
    void projectChanged(Project *project);

protected:
    void closeEvent(QCloseEvent *event) override;
    void setProject(Project *project);

protected:
    virtual Project *newProject(const QString &name, const QString &dirName) = 0;

private:
    void initMenuBar(void);
    void initToolBar(void);

private slots:
    void onFileNewProject(void);
    void onFileOpen(void);
    void onFileClose(void);
    void onFileSave(void);
    void onFileSaveAs(void);
    void onFileQuit(void);
    void onEditCut(void);
    void onEditCopy(void);
    void onEditPaste(void);
    void onViewWorld(bool checked);
    void onViewProperty(bool checked);
    void onViewResource(bool checked);
    void onViewOutput(bool checked);
    void onProjectImport(void);
    void onProjectExport(void);
    void onProjectBuild(void);
    void onProjectClean(void);
    void onToolsPreferences(void);
    void onHelpAbout(void);
    void onHelpAboutQt(void);
    void onWorldViewItemDoubleClicked(const QModelIndex &index);

private:
    Project *m_project;

    QMdiArea m_mdiArea;

    QDockWidget m_worldViewDocker;
    WorldView *m_worldView;

    QDockWidget m_propertyViewDocker;
    PropertyView *m_propertyView;

    QDockWidget m_resourceViewDocker;
    ResourceView *m_resourceView;

    QDockWidget m_outputViewDocker;
    OutputView *m_outputView;

    QMenu *m_fileMenu;
    QMenu *m_fileNewMenu;
    QAction *m_fileNewProject;
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
    QAction *m_viewProperty;
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
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_MAINWINDOWBASE_HPP
