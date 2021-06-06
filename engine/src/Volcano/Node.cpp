//
//
#include <string>
#include <vector>
#include <unordered_map>

#include <Volcano/Node.hpp>

VOLCANO_NODE_BEGIN

namespace Details {
    struct ConstructorInfo {
        ConstructorInfo(void):
            type(rttr::type::get<void>()),
            toVariant(nullptr)
        {
        }

        rttr::type type;
        Napi::FunctionReference ref;
        ToVariant toVariant;
    };

    using ConstructorInfoMap = std::unordered_map<rttr::type::type_id, ConstructorInfo>;

    struct Context {
        ConstructorInfoMap constructorInfoMap;
    };

    static Context *contextFromEnv(Napi::Env env)
    {
        auto context = env.GetInstanceData<Context>();
        if (context == nullptr) {
            context = new Context;
            env.SetInstanceData<Context>(context);
        }

        return context;
    }

    static rttr::variant toVariant(Napi::Object object)
    {
        VOLCANO_ASSERT(object.IsObject());

        auto &constructorInfoMap = contextFromEnv(object.Env())->constructorInfoMap;
        for (auto it = constructorInfoMap.begin(); it != constructorInfoMap.end(); ++it) {
            auto constructor = it->second.ref.Value();
            if (object.InstanceOf(constructor))
                return it->second.toVariant(object);
        }

        return rttr::variant();
    }

    static rttr::variant toVariant(Napi::Value value)
    {
        rttr::variant var;

        switch (value.Type()) {
        case napi_boolean:
            var = bool(value.ToBoolean());
            break;
        case napi_number:
            var = value.ToNumber().DoubleValue();
            break;
        case napi_string:
            var = std::string(value.ToString());
            break;
        case napi_object:
            var = toVariant(value.ToObject());
            break;
        case napi_function:
            var = Napi::Function(value.Env(), value);
            break;
        default:
            break;
        }

        return var;
    }

    static rttr::variant newInstance(rttr::constructor ctor, const Napi::CallbackInfo &info)
    {
        rttr::variant inst;

        switch (info.Length()) {
        case 0:
            inst = ctor.invoke();
            break;
        case 1:
            inst = ctor.invoke(toVariant(info[0]));
            break;
        case 2:
            inst = ctor.invoke(toVariant(info[0]), toVariant(info[1]));
            break;
        case 3:
            inst = ctor.invoke(toVariant(info[0]), toVariant(info[1]), toVariant(info[2]));
            break;
        case 4:
            inst = ctor.invoke(toVariant(info[0]), toVariant(info[1]), toVariant(info[2]), toVariant(info[3]));
            break;
        case 5:
            inst = ctor.invoke(toVariant(info[0]), toVariant(info[1]), toVariant(info[2]), toVariant(info[3]), toVariant(info[4]));
            break;
        case 6:
            inst = ctor.invoke(toVariant(info[0]), toVariant(info[1]), toVariant(info[2]), toVariant(info[3]), toVariant(info[4]), toVariant(info[5]));
            break;
        default:
            // inst = crot.invoke_variadic();
            break;
        }

        return inst;
    }

    rttr::variant newInstance(rttr::type type, const Napi::CallbackInfo &info)
    {
        rttr::variant inst;

        for (auto ctor : type.get_constructors()) {
            inst = newInstance(ctor, info);
            if (inst.is_valid())
                break;
        }

        return inst;
    }

    Napi::Function constructorFromType(Napi::Env env, rttr::type type)
    {
        auto &constructorInfoMap = contextFromEnv(env)->constructorInfoMap;
         auto it = constructorInfoMap.find(type.get_id());
        if (it == constructorInfoMap.end())
            return Napi::Function();

        return it->second.ref.Value();
    }

    static void makeInherits(Napi::Env env, Napi::Function ctor, Napi::Function superCtor)
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

    bool registerConstructor(Napi::Env env, rttr::type type, Napi::Function ctor, ToVariant toVariant)
    {
        VOLCANO_ASSERT(toVariant != nullptr);

        auto &constructorInfoMap = contextFromEnv(env)->constructorInfoMap;
        auto it = constructorInfoMap.find(type.get_id());
        if (it != constructorInfoMap.end())
            return false;

        auto baseClassTypes = type.get_base_classes();
        for (auto baseClassType : baseClassTypes)
            makeInherits(env, ctor, constructorFromType(env, baseClassType));

        auto &ci = constructorInfoMap[type.get_id()];
        ci.ref = Napi::Persistent(ctor);
        ci.ref.SuppressDestruct();
        ci.type = type;
        ci.toVariant = toVariant;

        return true;
    }
}

VOLCANO_NODE_END
