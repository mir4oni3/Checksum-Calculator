#include <catch2/catch_test_macros.hpp>
#include <future>
#include <chrono>

#include "Calculators/ChecksumCalculator.hpp"
#include "Observers/ObserverMessage.hpp"

class ChecksumCalculatorStub : public ChecksumCalculator {
public:
    bool getPause() const {
        return pause;
    }
    std::string calculate(std::istream&) const override {
        return "";
    }
    void wait() const {
        ChecksumCalculator::wait();
    }
};

TEST_CASE("TestChecksumCalculatorUpdate", "[ChecksumCalculator]") {
    ChecksumCalculatorStub calc;
    REQUIRE(calc.getPause() == false);

    REQUIRE_NOTHROW(calc.update(ObserverMessage::resumeCalculation, ""));
    REQUIRE(calc.getPause() == false);

    REQUIRE_NOTHROW(calc.update(ObserverMessage::pauseCalculation, ""));
    REQUIRE(calc.getPause());

    REQUIRE_NOTHROW(calc.update(ObserverMessage::resumeCalculation, ""));
    REQUIRE(calc.getPause() == false);
}

TEST_CASE("TestChecksumCalculatorWait", "[ChecksumCalculator]") {
    ChecksumCalculatorStub calc;

    //wait() should exit instantly when pause is false
    REQUIRE(calc.getPause() == false);
    auto asyncWait = std::async(std::launch::async, [&calc]() { calc.wait(); });
    if (asyncWait.wait_for(std::chrono::milliseconds(10)) != std::future_status::ready) {
        FAIL("Wait is taking too long when it shouldn't be waiting");
    }

    //set pause to true and run wait()
    calc.update(ObserverMessage::pauseCalculation, "");
    REQUIRE(calc.getPause());
    asyncWait = std::async(std::launch::async, [&calc]() { calc.wait(); });

    //ensure wait() is sleeping
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    //set pause to false and check if wait() exits on time with 20 ms margin
    calc.update(ObserverMessage::resumeCalculation, "");
    if (asyncWait.wait_for(std::chrono::milliseconds(CalcConstants::waitTime + 20)) == std::future_status::timeout) {
        FAIL("Wait did not resume in time");
    }
}