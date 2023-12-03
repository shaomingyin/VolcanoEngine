//
//
#ifndef VOLCANO_GAME_CONTEXT_HPP
#define VOLCANO_GAME_CONTEXT_HPP

#include <QObject>

#include <Volcano/View.hpp>
#include <Volcano/ResourceManager.hpp>
#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(QObject* parent = nullptr);

public:
    virtual ResourceManager& resourceManager() = 0;
    virtual View* view() { return nullptr; }
    void attachToQmlEngine(QQmlEngine& qml_engine);
    static Context* fromQmlEngine(const QQmlEngine& qml_engine);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXT_HPP
