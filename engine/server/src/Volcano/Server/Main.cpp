//
//
#include <QCoreApplication>

#include <Volcano/Server/Common.hpp>

VOLCANO_SERVER_BEGIN

static int Main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setOrganizationName("VolcanoEngine");
    QCoreApplication::setApplicationName("VolcanoServer");

    return QCoreApplication::exec();
}

VOLCANO_SERVER_END

int main(int argc, char *argv[])
{
    return Volcano::Server::Main(argc, argv);
}
