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
    ~Observable() = default;
    
    virtual void addObserver(const std::shared_ptr<Observer>& observer);
    virtual void removeObserver(const std::shared_ptr<Observer>& observer);
    virtual void notifyObservers(ObserverMessage message, const std::string& value) const;
};