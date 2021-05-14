//
//
#include <Volcano/Common.hpp>

VOLCANO_BEGIN

void makeLogger(std::shared_ptr<spdlog::logger> logger)
{
	VOLCANO_ASSERT(logger);

#ifdef VOLCANO_DEBUG
	logger->set_level(spdlog::level::debug);
#else
	logger->set_level(spdlog::level::info);
#endif

	logger->set_pattern("[Volcano%n] %^%L%$ %T.%e %t %v");
}

VOLCANO_END
