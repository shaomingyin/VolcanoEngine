//
//
#include <Volcano/Server/Common.hpp>

VOLCANO_SERVER_BEGIN

static int main(int argc, char *argv[])
{
	auto loop = uv_default_loop();

	uv_run(loop, UV_RUN_DEFAULT);

	return EXIT_SUCCESS;
}

VOLCANO_SERVER_END

int main(int argc, char *argv[])
{
	return Volcano::Server::main(argc, argv);
}
