#pragma once

#include "Observer.hpp"
#include "ObserverMessage.hpp"

#include <vector>
#include <memory>
#include <string>

class Observable {
protected:
    std::vector<std::shared_ptr<Observer>> observers;
public:
    void addObserver(const std::shared_ptr<Observer>& observer);
    void removeObserver(const std::shared_ptr<Observer>& observer);
    void notifyObservers(ObserverMessage message, const std::string& value) const;
};