#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <future>
#include <chrono>

#include "Calculators/SHA256Calculator.hpp"
#include "Observers/Observer.hpp"

class FailObserver : public Observer {
public:
    void update(ObserverMessage, const std::string&) override {
        FAIL("Observer should not be called");
    }
};

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

TEST_CASE("TestSHA256CalculatorWait", "[SHA256Calculator]") {
    SHA256Calculator calc;

    //pause the calculation
    calc.update(ObserverMessage::pauseCalculation, "");

    //since calculator notifies observers on progress, if
    //FailObserver gets notified, then the calculator is not paused(it should be)
    std::shared_ptr<FailObserver> failObserver = std::make_shared<FailObserver>();
    calc.addObserver(failObserver);

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
    calc.removeObserver(failObserver);
    
    //unpause the calculation
    calc.update(ObserverMessage::resumeCalculation, "");

    //wait for the calculation to complete
    REQUIRE_NOTHROW(asyncWait.get());
}