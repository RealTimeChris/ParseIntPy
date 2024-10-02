#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../MyModule/ParseInt.hpp"

namespace py = pybind11;

// Wrap your functions
int64_t parse_integer(const std::string& input) {
    int64_t newValue{};
    parseInt(newValue, input.data());
    return newValue;
}

std::vector<int64_t> parse_integers(const std::vector<std::string>& values) {
    std::vector<int64_t> results;
    results.reserve(values.size());
    for (const auto& val : values) {
        int64_t valueNew{};
        parseInt(valueNew, val.data());
        results.push_back(valueNew);
    }
    return results;
}

PYBIND11_MODULE(parseint, m) {
    m.def("parse_integers", &parse_integers, "Parse a list of integers from strings");
    m.def("parse_integer", &parse_integer, "A function that parses an integer from a string");
}

