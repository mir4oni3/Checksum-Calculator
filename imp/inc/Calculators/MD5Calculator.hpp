#pragma once

#include "ChecksumCalculator.hpp"


class MD5Calculator : public ChecksumCalculator {
    static const std::string md5Regex;
public:
    std::string calculate(std::istream&) const override;  
    std::regex getRegex() const override;
};
