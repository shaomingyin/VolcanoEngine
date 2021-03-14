//
//
#ifndef VOLCANO_GAME_CONTEXTIMPL_HPP
#define VOLCANO_GAME_CONTEXTIMPL_HPP

#include <string>
#include <string_view>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

class Context {
public:
    Context(void);
    virtual ~Context(void);

public:
    bool init(std::string_view rootDir);
    void update(float elapsed);
    const std::string &rootDir(void) const;
    World &world(void);

private:
    std::string m_rootDir;
    World m_world;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXTIMPL_HPP
