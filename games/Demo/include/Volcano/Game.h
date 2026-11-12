#ifndef VOLCANO_GAME_H
#define VOLCANO_GAME_H

#include <memory>

#include <Volcano/World/Context.h>
#include <Volcano/World/Scene.h>

// Implemented by the selected game; each call creates a fresh Scene.
std::unique_ptr<Volcano::World::Scene> volcanoCreateGame(Volcano::World::Context& context);

#endif // VOLCANO_GAME_H
