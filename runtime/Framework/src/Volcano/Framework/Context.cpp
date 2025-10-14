//
//
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

void Context::runAllTasks() noexcept {
    try {
        scheduler_.run_all_tasks();
    } catch (...) {
        // TODO
    }
}

VOLCANO_FRAMEWORK_END
