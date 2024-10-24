//
//
#include <Volcano/System/MultiPlayerClient.h>

VOLCANO_SYSTEM_BEGIN

MultiPlayerClient::MultiPlayerClient()
	: frontend_(resourceManager(), net_client_) {
}

VOLCANO_SYSTEM_END
