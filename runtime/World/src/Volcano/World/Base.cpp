//
//
#include <async++.h>
#include <edyn/edyn.hpp>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/World/Base.hpp>

VOLCANO_WORLD_BEGIN

Base::Base(Context& context)
    : context_(context) {
}

Base::~Base() {
}

VOLCANO_WORLD_END
