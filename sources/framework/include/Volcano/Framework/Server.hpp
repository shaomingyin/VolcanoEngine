//
//
#ifndef VOLCANO_FRAMEWORK_SERVER_HPP
#define VOLCANO_FRAMEWORK_SERVER_HPP

#include <memory>

#include <Volcano/Game/ContextBase.hpp>
#include <Volcano/Framework/Common.hpp>

VOLCANO_FRAMEWORK_BEGIN

class Server {
public:
    Server(void);
    virtual ~Server(void);

public:
    bool init(void);
    void frame(float elapsed);

private:
    std::unique_ptr<Game::ContextBase> m_game;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_SERVER_HPP
