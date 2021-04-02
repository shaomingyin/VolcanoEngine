//
//
#ifndef VOLCANO_GAME_CONTEXT_HPP
#define VOLCANO_GAME_CONTEXT_HPP

#include <string>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

class Context {
public:
    Context(void);
    virtual ~Context(void);

public:
    virtual bool init(void);
    virtual void frame(float elapsed);
    virtual std::string name(void);
    World *world(void);
    const World *world(void) const;
    void setWorld(World *p);
    Camera *camera(void);
    const Camera *camera(void) const;
    void setCamera(Camera *p);

public:
    sigslot::signal<World *> onWorldChanged;
    sigslot::signal<Camera *> onCameraChanged;

private:
    Entity *m_screen;
    World *m_world;
    Camera *m_camera;
};

extern Context *createContext(void);

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXT_HPP
