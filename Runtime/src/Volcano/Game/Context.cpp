//
//
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

static const char *CONTEXT_PROPERTY_KEY = "Volcano.Game.Context";

Context::Context(QObject *parent):
    QObject(parent)
{
}

Context::~Context(void)
{
}

void Context::attach(QQmlEngine *engine)
{
    Q_ASSERT(engine != nullptr);
    qDebug() << "Attach engine" << engine;
    engine->setProperty(CONTEXT_PROPERTY_KEY, QVariant::fromValue<Context *>(this));
}

void Context::attach(QQmlContext *context)
{
    Q_ASSERT(context != nullptr);
    attach(context->engine());
}

Context *Context::fromObject(QObject *object)
{
    if (object != nullptr) {
        QQmlEngine *eng = qmlEngine(object);
        qDebug() << "eng" << eng;
        if (eng != nullptr) {
            auto var = eng->property(CONTEXT_PROPERTY_KEY);
            qDebug() << "var" << var;
            if (var.canConvert<Context *>())
                return var.value<Context *>();
        }
    }

    return nullptr;
}

VOLCANO_GAME_END
