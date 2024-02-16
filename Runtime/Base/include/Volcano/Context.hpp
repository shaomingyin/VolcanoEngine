//
//
#ifndef VOLCANO_CONTEXT_HPP
#define VOLCANO_CONTEXT_HPP

#include <QObject>

#include <Volcano/Common.hpp>
#include <Volcano/View.hpp>
#include <Volcano/ResourceManager.hpp>

VOLCANO_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(View& view, QObject* parent = nullptr);

public:
    ResourceManager& resourceManager() {
        return resource_manager_;
    }

    View& view() {
        return view_;
    }

    void attachToQmlEngine(QQmlEngine& qml_engine);
    static Context* fromQmlEngine(const QQmlEngine& qml_engine);

private:
    ResourceManager resource_manager_;
    View& view_;
};

VOLCANO_END

#endif // VOLCANO_CONTEXT_HPP
