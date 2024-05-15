//
//
#include <Windows.h>

#include <QDebug>
#include <QLibrary>
#include <QPluginLoader>
#include <QGuiApplication>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    putenv("QT_DEBUG_PLUGIN=1");
    SetErrorMode(0);

    QLibrary lib("D:\\Projects\\ge\\VolcanoEngine\\Build\\Desktop_Qt_6_7_0_MSVC2019_64bit-Debug\\lib\\qtcreator\\plugins\\VolcanoEditor.dll");
    if (lib.load()) {
        qDebug() << "Load ok";
    } else {
        qDebug() << "Load failed: " << lib.errorString();
    }

    return app.exec();
}
