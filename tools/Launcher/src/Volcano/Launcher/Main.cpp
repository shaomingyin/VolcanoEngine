//
//
#include <memory>

#include <QGuiApplication>

#include <Volcano/Launcher/MainWindow.h>

int main(int argc, char* argv[]) {
    QCoreApplication::setOrganizationName("VolcanoEngine");
    QCoreApplication::setApplicationName("VolcanoLauncher");
    QCoreApplication::setApplicationVersion(Volcano::version.toString());

    QGuiApplication app(argc, argv);

    auto main_window = std::make_unique<Volcano::Launcher::MainWindow>();
    main_window->show();

    return app.exec();
}
