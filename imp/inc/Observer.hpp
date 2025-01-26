#pragma once

#include <string>

class Observer {
public:
    virtual void update(const std::string& message, const std::string& value) = 0;
};