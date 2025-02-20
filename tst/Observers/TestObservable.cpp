#include <catch2/catch_test_macros.hpp>

#include "Observers/Observable.hpp"
#include "Observers/Observer.hpp"

class TestObserver : public Observer {
public:
    ObserverMessage message;
    std::string value;

    void update(ObserverMessage message, const std::string& value) override {
        this->message = message;
        this->value = value;
    }
};

TEST_CASE("TestObservable", "[observable]") {
    Observable observable;
    TestObserver observer1;
    TestObserver observer2;

    REQUIRE_NOTHROW(observable.addObserver(observer1));
    REQUIRE_NOTHROW(observable.addObserver(observer2));

    REQUIRE_NOTHROW(observable.notifyObservers(ObserverMessage::progress, "test"));

    REQUIRE(observer1.message == ObserverMessage::progress);
    REQUIRE(observer1.value == "test");
    REQUIRE(observer2.message == ObserverMessage::progress);
    REQUIRE(observer2.value == "test");

    REQUIRE_NOTHROW(observable.removeObserver(observer1));

    REQUIRE_NOTHROW(observable.notifyObservers(ObserverMessage::resumeCalculation, "test2"));

    REQUIRE(observer1.message == ObserverMessage::progress);
    REQUIRE(observer1.value == "test");
    REQUIRE(observer2.message == ObserverMessage::resumeCalculation);
    REQUIRE(observer2.value == "test2");
}