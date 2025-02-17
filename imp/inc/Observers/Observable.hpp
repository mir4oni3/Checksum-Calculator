#pragma once

#include "Observer.hpp"
#include "ObserverMessage.hpp"

#include <vector>
#include <memory>
#include <string>

class Observable {
protected:
    //store raw pointers, since references are not reassignable
    //note that we cannot store unique_ptrs, since we don't own the observers
    std::vector<Observer*> observers;
public:
    ~Observable() = default;
    
    virtual void addObserver(Observer& observer);
    virtual void removeObserver(const Observer& observer);
    virtual void notifyObservers(ObserverMessage message, const std::string& value) const;
};