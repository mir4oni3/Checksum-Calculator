#pragma once

#include <fstream>
#include <functional>

namespace CalcConstants {
    const size_t chunkSize = 1024; // read 1 KB at a time
}

class ChecksumCalculator {
public:
    virtual std::string calculate(std::istream&) const = 0;
    virtual ~ChecksumCalculator() = default;
};