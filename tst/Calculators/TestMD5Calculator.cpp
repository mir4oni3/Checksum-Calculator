#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <future>
#include <chrono>

#include "Calculators/MD5Calculator.hpp"
#include "Observers/Observer.hpp"

class FailObserver : public Observer {
public:
    void update(ObserverMessage, const std::string&) override {
        FAIL("Observer should not be called");
    }
};

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

TEST_CASE("TestMD5CalculatorWait", "[MD5Calculator]") {
    MD5Calculator calc;

    //pause the calculation
    calc.update(ObserverMessage::pauseCalculation, "");

    //since calculator notifies observers on progress, if
    //FailObserver gets notified, then the calculator is not paused(it should be)
    std::unique_ptr<FailObserver> failObserver = std::make_unique<FailObserver>();
    calc.addObserver(*failObserver);

    //create a long string to force the calculator to read in chunks
    std::string longString(CalcConstants::chunkSize + 10, 'a');
    std::istringstream is(longString);

    //calc should wait for unpause to read the next chunk
    auto asyncWait = std::async(std::launch::async, [&calc, &is]() { 
        calc.calculate(is);
    });

    //wait to ensure the observer does not get notified during the pause
    asyncWait.wait_for(std::chrono::milliseconds(100));

    //remove observer
    calc.removeObserver(*failObserver);
    
    //unpause the calculation
    calc.update(ObserverMessage::resumeCalculation, "");

    //wait for the calculation to complete
    REQUIRE_NOTHROW(asyncWait.get());
}