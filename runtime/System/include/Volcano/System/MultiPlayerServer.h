//
//
#ifndef VOLCANO_SYSTEM_MULTIPLAYERSERVER_H
#define VOLCANO_SYSTEM_MULTIPLAYERSERVER_H

#include <Volcano/Net/Server.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

class MultiPlayerServer: public Base {
public:
	MultiPlayerServer() = default;
	virtual ~MultiPlayerServer() = default;

private:
	Net::Server net_server_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_MULTIPLAYERSERVER_H
