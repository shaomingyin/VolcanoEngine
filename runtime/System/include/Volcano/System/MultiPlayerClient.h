//
//
#ifndef VOLCANO_SYSTEM_MULTIPLAYERCLIENT_H
#define VOLCANO_SYSTEM_MULTIPLAYERCLIENT_H

#include <Volcano/Net/Client.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/FrontEnd.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

class MultiPlayerClient: public Base {
public:
	MultiPlayerClient();
	virtual ~MultiPlayerClient() = default;

private:
	Net::Client net_client_;
	Frontend frontend_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_MULTIPLAYERCLIENT_H
