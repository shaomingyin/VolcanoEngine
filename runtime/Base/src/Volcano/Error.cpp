//
//
#include <Volcano/Error.h>

VOLCANO_BEGIN

ErrorCategory errorCategory;

const char* ErrorCategory::name() const noexcept {
    return "Radium";
}

std::string ErrorCategory::message(int condition) const {
    static const char* errc_messages[] = {
        "Ok",
        "InvalidContext",
        "Invalid Parameter",
        "Invalid State",
        "Timeout",
        "Existed",
        "NotExisted",
        "NotSupported",
        "OutOfMemory",
        "OutOfResource"
    };

    if (0 <= condition && condition < VOLCANO_DIMOF(errc_messages)) {
        return errc_messages[condition];
    }

    return "Unknown";
}

VOLCANO_END
