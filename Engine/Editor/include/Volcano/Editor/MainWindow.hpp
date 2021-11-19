//
//
#ifndef VOLCANO_EDITOR_MAINWINDOW_HPP
#define VOLCANO_EDITOR_MAINWINDOW_HPP

#include <QMainWindow>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/WorldView.hpp>

VOLCANO_EDITOR_BEGIN

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    MainWindow(void);
    ~MainWindow(void) override;

public:
    bool init(void);

private:
    WorldView m_worldView;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_MAINWINDOW_HPP
