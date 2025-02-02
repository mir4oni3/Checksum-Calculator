#include <catch2/catch_test_macros.hpp>

#include "Factories/CalculatorFactory.hpp"
#include "Calculators/MD5Calculator.hpp"
#include "Calculators/SHA256Calculator.hpp"

TEST_CASE("TestCalculatorFactoryGetCalculator", "[CalculatorFactory]") {
    std::shared_ptr<ChecksumCalculator> calc;

    REQUIRE_NOTHROW(calc = ChecksumCalculatorFactory::getCalculator("md5"));
    REQUIRE(std::dynamic_pointer_cast<MD5Calculator>(calc));

    REQUIRE_NOTHROW(calc = ChecksumCalculatorFactory::getCalculator("sha256"));
    REQUIRE(std::dynamic_pointer_cast<SHA256Calculator>(calc));

    REQUIRE_THROWS_AS(ChecksumCalculatorFactory::getCalculator("unsupported"), std::invalid_argument);
}