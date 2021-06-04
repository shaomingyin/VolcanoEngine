//
//
#include <Volcano/Node.hpp>

VOLCANO_NODE_BEGIN

void makeInherits(Napi::Env env, Napi::Function ctor, Napi::Function superCtor)
{
    if (ctor.IsUndefined() || ctor.IsNull()) {
        throwError(env, "Invalid ctor");
        return;
    }

    if (superCtor.IsUndefined() || superCtor.IsNull()) {
        throwError(env, "invalid superCtor");
        return;
    }

    auto object = env.Global().Get("Object").ToObject();
    auto setPrototypeOf = Napi::Function(env, object.Get("setPrototypeOf"));

    auto protoType = ctor.Get("prototype");
    auto superProtoType = superCtor.Get("prototype");

    ctor["super_"] = superCtor;

    napi_value args[2] = {
        static_cast<napi_value>(protoType),
        static_cast<napi_value>(superProtoType)
    };

    setPrototypeOf.Call(2, args);
}

VOLCANO_NODE_END
