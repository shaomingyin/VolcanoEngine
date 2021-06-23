//
//
#include <QString>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QSurfaceFormat>

#include <physfs.h>

#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/World.hpp>

#include <Volcano/Game/Graphics/Common.hpp>
#include <Volcano/Game/Graphics/Camera.hpp>
#include <Volcano/Game/Graphics/Light.hpp>
#include <Volcano/Game/Graphics/DirectionalLight.hpp>
#include <Volcano/Game/Graphics/PointLight.hpp>
#include <Volcano/Game/Graphics/SpotLight.hpp>
#include <Volcano/Game/Graphics/Mesh.hpp>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

template <typename T>
static void registerUncreatableType(const char *uri, const char *name)
{
    qmlRegisterUncreatableType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, name,
        QString("Cannot create '%1' instance(s).").arg(name));
}

template <typename T>
static void registerType(const char *uri, const char *name)
{
    qmlRegisterType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, name);
}

bool init(void)
{
    const char *uri = "volcano.game";

    registerUncreatableType<Game::Component>(uri, "Component");
    registerUncreatableType<Game::Object>(uri, "Object");

    registerType<Game::Entity>(uri, "Entity");
    registerType<Game::World>(uri, "World");

    uri = "volcano.game.graphics";

    registerUncreatableType<Game::Graphics::Light>(uri, "Light");

    registerType<Game::Graphics::Camera>(uri, "Camera");
    registerType<Game::Graphics::DirectionalLight>(uri, "DirectionalLight");
    registerType<Game::Graphics::PointLight>(uri, "PointLight");
    registerType<Game::Graphics::SpotLight>(uri, "SpotLight");
    registerType<Game::Graphics::Mesh>(uri, "Mesh");

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    QSurfaceFormat format;
    Game::Graphics::glMakeDefaultSurfaceFormat(format);
    QSurfaceFormat::setDefaultFormat(format);

    return true;
}

VOLCANO_SYSTEM_END
