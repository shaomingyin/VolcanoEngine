//
//
#include <QByteArray>
#include <QSettings>

#include <Volcano/Editor/MainWindow.hpp>

VOLCANO_EDITOR_BEGIN

MainWindow::MainWindow(void)
{
}

MainWindow::~MainWindow(void)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
}

bool MainWindow::init(void)
{
    setCentralWidget(&m_mdiArea);

    QSettings settings;
    this->restoreGeometry(settings.value("geometry").toByteArray());

    return true;
}

VOLCANO_EDITOR_END
