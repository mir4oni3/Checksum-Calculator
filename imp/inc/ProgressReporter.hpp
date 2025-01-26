#pragma once

#include "Observer.hpp"

class ProgressReporter : public Observer {
    std::ostream& os;
public:
    ProgressReporter(std::ostream& reportTo);
    virtual ~ProgressReporter() = default;
    virtual void update(const std::string& message, const std::string& value) const override;
};