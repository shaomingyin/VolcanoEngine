//
//
#ifndef VOLCANO_EDITOR_MAINWINDOW_HPP
#define VOLCANO_EDITOR_MAINWINDOW_HPP

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/MainWindowBase.hpp>

VOLCANO_EDITOR_BEGIN

class MainWindow: public MainWindowBase {
    Q_OBJECT

public:
    MainWindow(void);
    ~MainWindow(void) override;

public:
    bool init(void) override;

protected:
    Project *newProject(const QString &name, const QString &dirName) override;

private:
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_MAINWINDOW_HPP
