#include "Observers/Observable.hpp"

void Observable::addObserver(Observer& observer) {
    observers.push_back(&observer);
}

void Observable::removeObserver(const Observer& observer) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == &observer) {
            observers.erase(it);
            break;
        }
    }
}

void Observable::notifyObservers(ObserverMessage message, const std::string& value) const {
    for (auto& observer : observers) {
        if (observer) {
            observer->update(message, value);
        }
    }
}