//
//
#include <QGuiApplication>

#include <Volcano/Launcher/MainWindow.h>

int main(int argc, char* argv[]) {
    QCoreApplication::setOrganizationName("VolcanoEngine");
    QCoreApplication::setApplicationName("VolcanoLauncher");
    QCoreApplication::setApplicationVersion(Volcano::version.toString());

    QGuiApplication app(argc, argv);

    return app.exec();
}
