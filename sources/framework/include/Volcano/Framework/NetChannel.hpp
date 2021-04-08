//
//
#ifndef VOLCANO_FRAMEWORK_NETCHANNEL_HPP
#define VOLCANO_FRAMEWORK_NETCHANNEL_HPP

#include <Volcano/Framework/Common.hpp>
#include <Volcano/Framework/Channel.hpp>

VOLCANO_FRAMEWORK_BEGIN

class NetChannel: public Channel {
public:
    NetChannel(void);
    ~NetChannel(void) override;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_NETCHANNEL_HPP
