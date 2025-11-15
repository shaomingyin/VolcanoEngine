//
//
#include <Volcano/Error.hpp>

VOLCANO_BEGIN

const char* ErrorCategory::name() const noexcept {
    return "Volcano.Error";
}

std::string ErrorCategory::message(int ev) const {
    if (ev == 1) {
        return "unknown error";
    }
    return "error: " + std::to_string(ev);
}

const ErrorCategory& errorCategory() {
    static ErrorCategory instance;
    return instance;
}

VOLCANO_END
