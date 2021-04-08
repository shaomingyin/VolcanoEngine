//
//
#ifndef VOLCANO_FRAMEWORK_LOCALCHANNEL_HPP
#define VOLCANO_FRAMEWORK_LOCALCHANNEL_HPP

#include <Volcano/Framework/Common.hpp>
#include <Volcano/Framework/Channel.hpp>

VOLCANO_FRAMEWORK_BEGIN

class LocalChannel: public Channel {
public:
    LocalChannel(void);
    ~LocalChannel(void) override;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_LOCALCHANNEL_HPP
