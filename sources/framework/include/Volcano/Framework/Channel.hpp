//
//
#ifndef VOLCANO_FRAMEWORK_CHANNEL_HPP
#define VOLCANO_FRAMEWORK_CHANNEL_HPP

#include <Volcano/IO.hpp>
#include <Volcano/Framework/Common.hpp>

VOLCANO_FRAMEWORK_BEGIN

class Channel: public IO {
public:
    Channel(void);
    ~Channel(void) override;

public:
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CHANNEL_HPP
