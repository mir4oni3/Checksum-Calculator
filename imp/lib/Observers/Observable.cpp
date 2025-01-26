#include "Observers/Observable.hpp"

void Observable::addObserver(const std::shared_ptr<Observer>& observer) {
    observers.push_back(observer);
}

void Observable::removeObserver(const std::shared_ptr<Observer>& observer) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == observer) {
            observers.erase(it);
            break;
        }
    }
}

void Observable::notifyObservers(const std::string& message, const std::string& value) const {
    for (auto& observer : observers) {
        observer->update(message, value);
    }
}