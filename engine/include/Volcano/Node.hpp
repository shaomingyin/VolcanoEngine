//
//
#ifndef VOLCANO_NODE_HPP
#define VOLCANO_NODE_HPP

#include <uv.h>

#include <Volcano/Common.hpp>

VOLCANO_INLINE void uv_close_sync(uv_handle_t *p)
{
    bool closed = false;
    p->data = &closed;

    uv_close(p, [](uv_handle_t *p) {
        auto closed = reinterpret_cast<bool *>(p->data);
        *closed = true;
    });

    while (!closed)
        uv_run(p->loop, UV_RUN_ONCE);
}

VOLCANO_INLINE void uv_close_sync(uv_idle_t *p)
{
    uv_close_sync(reinterpret_cast<uv_handle_t *>(p));
}

VOLCANO_INLINE void uv_close_sync(uv_prepare_t *p)
{
    uv_close_sync(reinterpret_cast<uv_handle_t *>(p));
}

VOLCANO_INLINE void uv_close_sync(uv_check_t *p)
{
    uv_close_sync(reinterpret_cast<uv_handle_t *>(p));
}

VOLCANO_INLINE void uv_close_sync(uv_timer_t *p)
{
    uv_close_sync(reinterpret_cast<uv_handle_t *>(p));
}

VOLCANO_INLINE void uv_close_sync(uv_async_t *p)
{
    uv_close_sync(reinterpret_cast<uv_handle_t *>(p));
}

#define NAPI_VERSION 7
#include <node_api.h>

#define NODE_ADDON_API_DISABLE_DEPRECATED
#include <napi.h>

#define VOLCANO_NODE_BEGIN VOLCANO_BEGIN namespace Node {
#define VOLCANO_NODE_END } VOLCANO_END

VOLCANO_NODE_BEGIN

inline void throwError(Napi::Env env, std::string_view message)
{
    Napi::Error::New(env, message.data()).ThrowAsJavaScriptException();
}

inline Napi::Object newInstance(Napi::Env env, Napi::Function ctor, const std::initializer_list<napi_value> &args)
{
    Napi::EscapableHandleScope scope(env);
    Napi::Object object = ctor.New(args);
    return scope.Escape(napi_value(object)).ToObject();
}

void makeInherits(Napi::Env env, Napi::Function ctor, Napi::Function superCtor);

template <typename T>
class Object: public Napi::ObjectWrap<T> {
public:
    Object(const Napi::CallbackInfo &info):
        Napi::ObjectWrap<T>(info)
    {
    }

    ~Object(void) override
    {
    }

public:
    static Napi::Function constructor(Napi::Env env)
    {
        return constructorFromTypeId(env, typeId<T>());
    }

    static Napi::Object newInstance(Napi::Env env,
        const std::initializer_list<napi_value> &args = {})
    {
        return ::Volcano::Node::newInstance(env, constructor(env), args);
    }

protected:
    static Napi::Function defineClass(Napi::Env env, const char *name,
        const std::vector<Napi::ClassPropertyDescriptor<T>> &properties)
    {
        auto ctor = constructor(env);
        if (ctor.IsFunction())
            return ctor;

        ctor = Napi::ObjectWrap<T>::DefineClass(env, name, properties);
        registerConstructor(env, ctor);

        return ctor;
    }

    template <typename SUPER>
    static Napi::Function defineClass(Napi::Env env, const char *name,
        const std::vector<Napi::ClassPropertyDescriptor<T>> &properties)
    {
        auto ctor = defineClass(env, name, properties);
        makeInherits(env, ctor, constructorFromType<SUPER>(env));
        return ctor;
    }

private:
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

    static bool registerConstructor(Napi::Env env, TypeId id, Napi::Function ctor)
    {
        auto ctorMap = constructorMap(env);
        if (ctorMap == nullptr)
            return false;

        auto it = ctorMap->find(id);
        if (it != ctorMap->end())
            return false;

        auto ref = Napi::Persistent(ctor);
        ref.SuppressDestruct();

        (*ctorMap)[id] = std::move(ref);

        return true;
    }

    static bool registerConstructor(Napi::Env env, Napi::Function ctor)
    {
        return registerConstructor(env, typeId<T>(), ctor);
    }

    static Napi::Function constructorFromTypeId(Napi::Env env, TypeId id)
    {
        auto ctorMap = constructorMap(env);
        if (ctorMap == nullptr)
            return Napi::Function();

        auto it = ctorMap->find(id);
        if (it == ctorMap->end())
            return Napi::Function();

        return it->second.Value();
    }

    template <typename U>
    Napi::Function constructorFromType(Napi::Env env)
    {
        return constructorFromTypeId(env, typeId<U>());
    }
};

VOLCANO_NODE_END

#endif // VOLCANO_NODE_HPP
