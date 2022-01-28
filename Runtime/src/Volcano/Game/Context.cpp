//
//
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

#ifdef VOLCANO_DEBUG
static const char *CONTEXT_PROPERTY_KEY = "Volcano.Game.Context";
#else
static const char *CONTEXT_PROPERTY_KEY = "_VGC_";
#endif

Context::Context(QObject *parent):
    QObject(parent)
{
}

Context::~Context(void)
{
}

Context *Context::fromObject(QObject *object)
{
    Context *context = nullptr;

    if (object != nullptr) {
        auto engine = qmlEngine(object);
        if (engine != nullptr) {
            auto var = engine->property(CONTEXT_PROPERTY_KEY);
            if (var.canConvert<Context *>())
                context = var.value<Context *>();
        }
    }

    return context;
}

void Context::attach(QQmlEngine *engine)
{
    Q_ASSERT(engine != nullptr);
    engine->setProperty(CONTEXT_PROPERTY_KEY, QVariant::fromValue(this));
}

VOLCANO_GAME_END
