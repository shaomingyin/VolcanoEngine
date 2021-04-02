//
//
#ifndef VOLCANO_FRAMEWORK_SINGLEPLAYER_HPP
#define VOLCANO_FRAMEWORK_SINGLEPLAYER_HPP

#include <Volcano/Framework/Common.hpp>
#include <Volcano/Framework/Server.hpp>
#include <Volcano/Framework/Client.hpp>

VOLCANO_FRAMEWORK_BEGIN

class SinglePlayer: public Client {
public:
    SinglePlayer(void);
    ~SinglePlayer(void) override;

public:
    bool init(void) override;
    void frame(float elapsed) override;

private:
    Server m_server;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_SINGLEPLAYER_HPP
