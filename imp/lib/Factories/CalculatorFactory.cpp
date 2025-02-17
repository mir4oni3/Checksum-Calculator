#include "Factories/CalculatorFactory.hpp"
#include "Calculators/MD5Calculator.hpp"
#include "Calculators/SHA256Calculator.hpp"

std::unique_ptr<ChecksumCalculator> ChecksumCalculatorFactory::getCalculator(const std::string& algorithm) {
    if (algorithm == "md5") {
        return std::make_unique<MD5Calculator>();
    }
    if (algorithm == "sha256") {
        return std::make_unique<SHA256Calculator>();
    }
    throw std::invalid_argument("ChecksumCalculatorFactory::getCalculator - Unsupported algorithm");
    return nullptr;
}