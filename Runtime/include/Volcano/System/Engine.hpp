//
//
#ifndef VOLCANO_SYSTEM_ENGINE_HPP
#define VOLCANO_SYSTEM_ENGINE_HPP

#include <Volcano/ResourceManager.hpp>
#include <Volcano/Game/Context.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class Engine: public Game::Context {
    Q_OBJECT

public:
    Engine(QObject* parent = nullptr);

public:
    virtual bool init();
    ResourceManager& resourceManager() override;

private:
    ResourceManager resourceManager_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_ENGINE_HPP
