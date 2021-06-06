//
//
#ifndef VOLCANO_NODE_HPP
#define VOLCANO_NODE_HPP

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <uv.h>
#include <rttr/type>

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

namespace Details {
    using ToVariant = rttr::variant(*)(Napi::Object);

    inline rttr::type typeFromId(rttr::type::type_id id)
    {
        auto ret = rttr::type::get<void>();
        *(reinterpret_cast<uintptr_t *>(&ret)) = id;
        return ret;
    }

    rttr::variant newInstance(Napi::Env env, rttr::type type, const Napi::CallbackInfo &info);
    Napi::Function constructorFromType(Napi::Env env, rttr::type type);
    bool registerConstructor(Napi::Env env, rttr::type type, Napi::Function ctor, ToVariant toVariant);
}

inline void throwError(Napi::Env env, std::string_view message)
{
    Napi::Error::New(env, message.data()).ThrowAsJavaScriptException();
}

template <typename T>
class Object: public Napi::ObjectWrap<T> {
public:
    Object(const Napi::CallbackInfo &info):
        Napi::ObjectWrap<T>(info)
    {
        m_instance = Details::newIntance(rttr::type::get<T>(), info);
    }

    ~Object(void) override
    {
    }

public:
    static Napi::Function constructor(Napi::Env env)
    {
        return Details::constructorFromType(env, rttr::type::get<T>());
    }

    static Napi::ObjectReference newInstance(Napi::Env env,
        const std::initializer_list<napi_value> &args = {})
    {
        auto ctor = Details::constructorFromType(env, rttr::type::get<T>());
        return Napi::Persistent(ctor.New(args));
    }

    static Napi::Function registerConstructor(Napi::Env env)
    {
        auto type = rttr::type::get<T>();
        auto ctor = Details::constructorFromType(env, type);
        if (ctor.IsFunction())
            return Napi::Function();

        std::vector<Napi::ClassPropertyDescriptor<T>> properties;

        for (auto method : type.get_methods()) {
            if (method.get_access_level() != rttr::access_levels::public_access)
                continue;
            properties.push_back(InstanceMethod<&T::method>(
                method.get_name().data(), napi_default, (void *)(type.get_id())));
        }

        for (auto property : type.get_properties()) {
            if (property.get_access_level() != rttr::access_levels::public_access)
                continue;
            if (property.is_readonly()) {
                properties.push_back(InstanceAccessor<&T::getter>(
                    property.get_name().data(), napi_default, (void *)(type.get_id())));
            } else {
                properties.push_back(InstanceAccessor<&T::getter, &T::setter>(
                    property.get_name().data(), napi_default, (void *)(type.get_id())));
            }
        }

        return Details::registerConstructor(env, type,
            DefineClass(env, type.get_name().data(), properties, (void *)(type.get_id())),
            &Object::toVariant);
    }

private:
    Napi::Value getter(const Napi::CallbackInfo &info)
    {
    }

    void setter(const Napi::CallbackInfo &info)
    {
    }

    Napi::Value method(const Napi::CallbackInfo &info)
    {
    }

    static rttr::variant toVariant(Napi::Object object)
    {
        return Unwrap(object)->m_instance;
    }

private:
    rttr::variant m_instance;
};

VOLCANO_NODE_END

#endif // VOLCANO_NODE_HPP
