#pragma once

#include "ChecksumCalculator.hpp"

class SHA256Calculator : public ChecksumCalculator {
    static const std::string SHA256Regex;
public:
    std::string calculate(std::istream&) const override;  
    std::regex getRegex() const override;
};