#include <catch2/catch_test_macros.hpp>

#include "Observers/Observable.hpp"
#include "Observers/ProgressReporter.hpp"

#include <sstream>

class ProgressReporterTest : public ProgressReporter {
public:
    ProgressReporterTest(std::ostream& reportTo) : ProgressReporter(reportTo) {}

    size_t getTotalBytes() {
        return totalBytes;
    }
    bool getTotalBytesKnown() {
        return totalBytesKnown;
    }
    size_t getCurrentBytesRead() {
        return currentBytesRead;
    }
    std::chrono::time_point<std::chrono::system_clock> getStartTime() {
        return startTime;
    }
    bool getTimerStarted() {
        return timerStarted;
    }
};

TEST_CASE("TestProgressReporter", "[progressReporter]") {
    std::ostringstream os;
    ProgressReporterTest reporter(os);
    Observable observable;
    observable.addObserver(reporter);

    SECTION("Normal construction") {
        REQUIRE_NOTHROW(ProgressReporter(os));
    }

    SECTION("Invalid stream") {
        os.setstate(std::ios::failbit);
        REQUIRE_THROWS_AS(ProgressReporter(os), std::invalid_argument);
    }

    SECTION("resumeCalculation") {
        observable.notifyObservers(ObserverMessage::resumeCalculation, "test");
        REQUIRE(os.str() == "Processing test...\n");
    }

    SECTION("rootFileSize") {
        REQUIRE(reporter.getTotalBytes() == 0);
        REQUIRE(!reporter.getTotalBytesKnown());
        observable.notifyObservers(ObserverMessage::rootFileSize, "100");
        REQUIRE(reporter.getTotalBytes() == 100);
        REQUIRE(reporter.getTotalBytesKnown());
    }
    
    SECTION("progress") {
        REQUIRE(reporter.getCurrentBytesRead() == 0);
        observable.notifyObservers(ObserverMessage::rootFileSize, "100");
        observable.notifyObservers(ObserverMessage::progress, "50");
        REQUIRE(reporter.getCurrentBytesRead() == 50);
    }

    SECTION("newRegularFile") {
        observable.notifyObservers(ObserverMessage::newRegularFile, "test2");
        REQUIRE(os.str().find("Processing test2...\n") != std::string::npos);
        REQUIRE(reporter.getStartTime() != std::chrono::time_point<std::chrono::system_clock>());
        REQUIRE(reporter.getTimerStarted());
    }

    SECTION("allFilesHandled") {
        observable.notifyObservers(ObserverMessage::allFilesHandled, "");
        REQUIRE(reporter.getTotalBytes() == 0);
        REQUIRE(!reporter.getTotalBytesKnown());
        REQUIRE(reporter.getCurrentBytesRead() == 0);
        REQUIRE(!reporter.getTimerStarted());
    }
}

TEST_CASE("TestProgressReporterVisualizeBar", "[progressReporter]") {
    std::ostringstream os1;
    ProgressReporterTest reporter(os1);

    REQUIRE_THROWS_AS(reporter.visualizeProgressBar(), std::runtime_error);
    
    reporter.update(ObserverMessage::rootFileSize, "10000000");
    reporter.update(ObserverMessage::progress, "5000000");
    
    size_t expectedFillCount = ReporterConstants::progressBarWidth / 2;

    std::string expected = "[" + std::string(expectedFillCount, '#');
    expected += std::string(ReporterConstants::progressBarWidth - expectedFillCount, '.');
    expected += "] 5000000B / 10000000B, 50% ready, remaining time: ";
    
    std::string result = os1.str();
    REQUIRE(result.find(expected) != std::string::npos);
}