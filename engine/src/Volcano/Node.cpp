//
//
#include <string>
#include <unordered_map>

#include <Volcano/Node.hpp>

VOLCANO_NODE_BEGIN

using ConstructorMap = std::unordered_map<TypeId, Napi::FunctionReference>;

static ConstructorMap *constructorMap(Napi::Env env)
{
    auto ctorMap = env.GetInstanceData<ConstructorMap>();
    if (ctorMap == nullptr) {
        ctorMap = new ConstructorMap;
        env.SetInstanceData<ConstructorMap>(ctorMap);
    }

    return ctorMap;
}

bool registerConstructor(Napi::Env env, TypeId id, Napi::Function ctor)
{
    auto ctorMap = constructorMap(env);
    if (ctorMap == nullptr)
        return false;

    auto it = ctorMap->find(id);
    if (it != ctorMap->end())
        return false;

    (*ctorMap)[id] = Napi::Persistent(ctor);

    return true;
}

Napi::Function constructor(Napi::Env env, TypeId id)
{
    auto ctorMap = constructorMap(env);
    if (ctorMap == nullptr)
        return Napi::Function();

    auto it = ctorMap->find(id);
    if (it == ctorMap->end())
        return Napi::Function();

    return it->second.Value();
}

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
