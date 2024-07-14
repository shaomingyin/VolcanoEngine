//
//
#ifndef VOLCANO_GAME_CONTEXT_HPP
#define VOLCANO_GAME_CONTEXT_HPP

#include <QObject>

#include <Volcano/ResourceManager.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API Context: public QObject {
    Q_OBJECT

public:
    Context(QObject* parent = nullptr);

public:
    virtual ResourceManager& resourceManager() = 0;
    virtual bool beginView() = 0;
    virtual void endView() = 0;
    virtual Graphics::View& view() = 0;

public:
    void attachToQmlEngine(QQmlEngine& qml_engine);
    static Context* fromQmlEngine(const QQmlEngine& qml_engine);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXT_HPP
