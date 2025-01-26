#pragma once

#include "ChecksumCalculator.hpp"

class SHA256Calculator : public ChecksumCalculator {
public:
    std::string calculate(std::istream&) const override;  
};