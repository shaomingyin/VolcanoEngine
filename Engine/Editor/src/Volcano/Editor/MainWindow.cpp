//
//
#include <QSettings>

#include <Volcano/Editor/MainWindow.hpp>

VOLCANO_EDITOR_BEGIN

MainWindow::MainWindow(void)
{
}

MainWindow::~MainWindow(void)
{
    QSettings settings;
    settings.setValue("MainWindow/State", saveState());
    settings.setValue("MainWindow/Geometry", saveGeometry());
}

bool MainWindow::init(void)
{
    setCentralWidget(&m_worldView);

    QSettings settings;

    if (settings.contains("MainWindow/State"))
        restoreState(settings.value("MainWindow/State").toByteArray());

    if (settings.contains("MainWindow/Geometry"))
        restoreGeometry(settings.value("MainWindow/Geometry").toByteArray());
    else
        resize(800, 600);

    return true;
}

VOLCANO_EDITOR_END
