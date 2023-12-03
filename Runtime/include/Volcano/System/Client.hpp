//
//
#ifndef VOLCANO_SYSTEM_CLIENT_HPP
#define VOLCANO_SYSTEM_CLIENT_HPP

#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/Engine.hpp>

VOLCANO_SYSTEM_BEGIN

class Client: public Engine {
    Q_OBJECT

public:
    Client(QObject* parent = nullptr);

private:
    Graphics::Renderer graphicsRenderer_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CLIENT_HPP
