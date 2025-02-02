#include <catch2/catch_test_macros.hpp>

#include <sstream>

#include "Calculators/MD5Calculator.hpp"

//the following test cases are based on the md5sum bash command results
//echo -n "str" | md5sum
TEST_CASE("TestMD5CalculatorCalculate", "[MD5Calculator]") {
    MD5Calculator calc;
    std::string result;

    std::istringstream is("test");
    REQUIRE_NOTHROW(result = calc.calculate(is));
    REQUIRE(result == "098f6bcd4621d373cade4e832627b4f6");

    is.str("This is a very nice string! :)");
    REQUIRE_NOTHROW(result = calc.calculate(is));
    REQUIRE(result == "e3397317be70cd7daa757fcb8be708c6");
}