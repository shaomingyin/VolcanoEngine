//
//
#ifndef VOLCANO_NODE_HPP
#define VOLCANO_NODE_HPP

#include <string_view>

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

bool registerConstructor(Napi::Env env, TypeId id, Napi::Function ctor);
Napi::Function constructor(Napi::Env env, TypeId id);
void makeInherits(Napi::Env env, Napi::Function ctor, Napi::Function superCtor);

template <typename T>
bool registerConstructor(Napi::Env env, Napi::Function ctor)
{
    return registerConstructor(env, typeId<T>(), ctor);
}

template <typename T>
Napi::Function constructor(Napi::Env env)
{
    return constructor(env, typeId<T>());
}

inline void throwError(Napi::Env env, std::string_view message)
{
    Napi::Error::New(env, message.data()).ThrowAsJavaScriptException();
}

inline Napi::Object newInstance(Napi::Function ctor, const std::initializer_list<napi_value> &args)
{
    Napi::EscapableHandleScope scope(ctor.Env());
    Napi::Object object = ctor.New(args);
    return scope.Escape(napi_value(object)).ToObject();
}

template <typename T>
Napi::Function defineClass(Napi::Env env, std::string_view name,
    const std::vector<Napi::ClassPropertyDescriptor<T>> &properties)
{
    auto ctor = constructor<T>(env);
    if (ctor.IsFunction())
        return ctor;

    ctor = Napi::ObjectWrap<T>::DefineClass(env, name.data(), properties);
    registerConstructor<T>(env, ctor);

    return ctor;
}

template <typename T, typename SUPER>
Napi::Function defineClass(Napi::Env env, std::string_view name,
    const std::vector<Napi::ClassPropertyDescriptor<T>> &properties)
{
    auto ctor = defineClass<T>(env, name, properties);
    makeInherits(ctor, constructor<SUPER>(env));
    return ctor;
}

VOLCANO_NODE_END

#endif // VOLCANO_NODE_HPP
