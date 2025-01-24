#pragma once

#include "ChecksumCalculator.hpp"

#include <memory>

class ChecksumCalculatorFactory {
public:
    static std::shared_ptr<ChecksumCalculator> getCalculator(const std::string& algorithm);
};