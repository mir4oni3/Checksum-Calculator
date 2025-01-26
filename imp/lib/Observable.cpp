#include "../inc/Observable.hpp"

void Observable::addObserver(const std::shared_ptr<Observer>& observer) {
    observers.push_back(observer);
}

void Observable::removeObserver(const std::shared_ptr<Observer>& observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Observable::notifyObservers(const std::string& message, const std::string& value) const {
    for (auto& observer : observers) {
        observer->update(message, value);
    }
}