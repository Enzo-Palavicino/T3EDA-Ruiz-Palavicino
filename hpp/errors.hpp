#ifndef EDACAL_ERRORS_HPP
#define EDACAL_ERRORS_HPP

#include <stdexcept>
#include <string>

namespace edacal {

class EdaError : public std::runtime_error {
public:
    explicit EdaError(const std::string& message)
        : std::runtime_error(message) {}
};

} // namespace edacal

#endif
