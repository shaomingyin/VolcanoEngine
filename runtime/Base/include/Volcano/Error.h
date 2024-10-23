//
//
#ifndef VOLCANO_ERROR_HPP
#define VOLCANO_ERROR_HPP

#include <stdexcept>
#include <system_error>

#include <Volcano/Common.h>

VOLCANO_BEGIN

enum class Errc {
    Ok,

    InvalidContext,
    InvalidParameter,
    InvalidState,
    Timeout,
    Existed,
    NotExisted,
    NotSupported,

    Unknown
};

class ErrorCategory: public std::error_category {
public:
    const char* name() const noexcept override;
    std::string message(int condition) const override;
};

extern ErrorCategory errorCategory;

class ErrorCondition: public std::error_condition {
public:
    ErrorCondition(Errc errc)
        : std::error_condition(static_cast<int>(errc), errorCategory) {
    }
};

class Error: public std::runtime_error {
public:
    Error(Errc errc)
        : std::runtime_error(errorCategory.message(static_cast<int>(errc)))
        , error_condition_(errc) {
    }

public:
    const ErrorCondition& condition() const {
        return error_condition_;
    }

private:
    ErrorCondition error_condition_;
};

VOLCANO_END

#endif // VOLCANO_ERROR_HPP
