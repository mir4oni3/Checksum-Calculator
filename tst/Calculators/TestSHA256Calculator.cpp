#include <catch2/catch_test_macros.hpp>

#include <sstream>

#include "Calculators/SHA256Calculator.hpp"

//the following test cases are based on the sha256sum bash command results
//echo -n "str" | sha256sum
TEST_CASE("TestSHA256CalculatorCalculate", "[SHA256Calculator]") {
    SHA256Calculator calc;
    std::string result;

    std::istringstream is("test");
    REQUIRE_NOTHROW(result = calc.calculate(is));
    REQUIRE(result == "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08");

    is.str("This is a very nice string! :)");
    REQUIRE_NOTHROW(result = calc.calculate(is));
    REQUIRE(result == "e28a8e7d88d7f657472b14330aac34352810cc96936455944c59aeabf705be47");
}