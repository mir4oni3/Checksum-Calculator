#pragma once

#include "MD5Calculator.hpp"
#include "SHA256Calculator.hpp"

#include <memory>

class ChecksumCalculatorFactory {
public:
    static std::shared_ptr<ChecksumCalculator> getCalculator(const std::string& algorithm);
};