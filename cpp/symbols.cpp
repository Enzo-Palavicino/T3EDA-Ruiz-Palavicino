#include "symbols.hpp"

namespace edacal {

SymbolTable::SymbolTable() {
    symbols_["ans"] = 0.0;
}

bool SymbolTable::has(const std::string& name) const {
    return symbols_.find(name) != symbols_.end();
}

double SymbolTable::get(const std::string& name) const {
    auto it = symbols_.find(name);
    if (it == symbols_.end()) {
        throw EdaError("variable no definida: " + name);
    }
    return it->second;
}

void SymbolTable::set(const std::string& name, double value) {
    symbols_[name] = value;
}

} // namespace edacal
