#ifndef EDACAL_SYMBOLS_HPP
#define EDACAL_SYMBOLS_HPP

#include "errors.hpp"

#include <string>
#include <unordered_map>

namespace edacal {

class SymbolTable {
public:
    SymbolTable();

    bool has(const std::string& name) const;
    double get(const std::string& name) const;
    void set(const std::string& name, double value);

private:
    std::unordered_map<std::string, double> symbols_;
};

} // namespace edacal

#endif
