#pragma once

#include "ObserverMessage.hpp"

#include <string>

class Observer {
public:
    virtual void update(ObserverMessage message, const std::string& value) = 0;
};