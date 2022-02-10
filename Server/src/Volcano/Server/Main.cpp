//
//
#include <QCoreApplication>

#include <Volcano/Server/Common.hpp>

VOLCANO_SERVER_BEGIN

static int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    setApplicationName("Server");

    return EXIT_SUCCESS;
}

VOLCANO_SERVER_END

int main(int argc, char *argv[])
{
    return Volcano::Server::main(argc, argv);
}
