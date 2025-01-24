#include "../inc/CalculatorFactory.hpp"
#include "../inc/MD5Calculator.hpp"
#include "../inc/SHA256Calculator.hpp"

std::shared_ptr<ChecksumCalculator> ChecksumCalculatorFactory::getCalculator(const std::string& algorithm) {
    if (algorithm == "md5") {
        return std::make_shared<MD5Calculator>();
    }
    if (algorithm == "sha256") {
        return std::make_shared<SHA256Calculator>();
    }
    throw std::invalid_argument("ChecksumCalculatorFactory::getCalculator - Unsupported algorithm");
    return nullptr;
}