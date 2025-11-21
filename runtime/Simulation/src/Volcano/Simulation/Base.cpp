//
//
#include <async++.h>
#include <edyn/edyn.hpp>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Simulation/Base.hpp>

VOLCANO_SIMULATION_BEGIN

Base::Base(Context& context)
    : context_(context) {
}

Base::~Base() {
}

VOLCANO_SIMULATION_END
