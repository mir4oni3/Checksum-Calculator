#include "MD5Calculator.hpp"
#include "SHA256Calculator.hpp"

class ChecksumCalculatorFactory {
public:
    static std::shared_ptr<ChecksumCalculator> getCalculator(const std::string& algorithm);
};