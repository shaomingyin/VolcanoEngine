//
//
#ifndef VOLCANO_FRAMEWORK_MULTIPLAYER_HPP
#define VOLCANO_FRAMEWORK_MULTIPLAYER_HPP

#include <Volcano/Framework/Common.hpp>
#include <Volcano/Framework/Client.hpp>

VOLCANO_FRAMEWORK_BEGIN

class MultiPlayer: public Client {
public:
    MultiPlayer(void);
    ~MultiPlayer(void) override;

public:
    bool init(void) override;
    void frame(float elapsed) override;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_MULTIPLAYER_HPP
