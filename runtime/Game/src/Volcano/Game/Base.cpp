//
//
#include <async++.h>
#include <edyn/edyn.hpp>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Game/Base.hpp>

VOLCANO_GAME_BEGIN

Base::Base(Context& context)
    : context_(context) {
}

Base::~Base() {
}

VOLCANO_GAME_END
