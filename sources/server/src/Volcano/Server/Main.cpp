//
//
#include <uv.h>

#include <Volcano/Server/Common.hpp>

VOLCANO_SERVER_BEGIN

static int Main(int argc, char *argv[])
{
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    return EXIT_SUCCESS;
}

VOLCANO_SERVER_END

int main(int argc, char *argv[])
{
    return Volcano::Server::Main(argc, argv);
}
