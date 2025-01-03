//
//
#include <memory>

#include <QString>
#include <QCoreApplication>
#include <QCommandLineParser>

#include <Volcano/ResourceBuilder/Common.h>
#include <Volcano/ResourceBuilder/MapCompiler.h>
#include <Volcano/ResourceBuilder/ModelCompiler.h>
#include <Volcano/ResourceBuilder/Packager.h>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    QCommandLineParser cmdline;

    cmdline.process(app);

    std::unique_ptr<Volcano::ResourceBuilder::Tool> tool;

    return app.exec();
}
