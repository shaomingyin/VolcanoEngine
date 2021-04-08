//
//
#ifndef VOLCANO_FRAMEWORK_CLIENT_HPP
#define VOLCANO_FRAMEWORK_CLIENT_HPP

#include <memory>

#include <Volcano/Game/ContextBase.hpp>
#include <Volcano/Drivers/Graphics/Memory.hpp>
#include <Volcano/Drivers/Graphics/Renderer.hpp>
#include <Volcano/Drivers/Input/Context.hpp>
#include <Volcano/Framework/Common.hpp>

VOLCANO_FRAMEWORK_BEGIN

class Client: public Game::ContextBase {
public:
    Client(void);
    ~Client(void) override;

public:
    bool init(void) override;
    void frame(float elapsed) override;

protected:
    virtual Drivers::Input::Context *createInputContext(void) = 0;

private:
    Drivers::Graphics::Memory m_graphicsMemory;
    Drivers::Graphics::Renderer m_graphicsRenderer;
    std::unique_ptr<Drivers::Input::Context> m_inputContext;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CLIENT_HPP
