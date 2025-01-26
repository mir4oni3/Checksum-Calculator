#pragma once

#include "ChecksumCalculator.hpp"

class MD5Calculator : public ChecksumCalculator {
public:
    std::string calculate(std::istream&) const override;  
};
