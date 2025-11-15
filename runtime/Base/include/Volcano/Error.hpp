//
//
#ifndef VOLCANO_ERROR_HPP
#define VOLCANO_ERROR_HPP

#include <string>
#include <system_error>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

enum Errc {
    Unknown = 1
};

class ErrorCategory : public std::error_category {
public:
    const char* name() const noexcept override;
    std::string message(int ev) const override;
};

const ErrorCategory& errorCategory();

inline std::error_code UnknownError() {
    return std::error_code(Errc::Unknown, errorCategory());
}

VOLCANO_END

#endif // VOLCANO_ERROR_HPP
