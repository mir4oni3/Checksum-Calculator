#pragma once

#include "Observer.hpp"
#include "ObserverMessage.hpp"

class ProgressReporter : public Observer {
    std::ostream& os;
    mutable size_t totalSize = 0;
    mutable bool isTotalSizeSet = false;
public:
    ProgressReporter(std::ostream& reportTo);
    virtual ~ProgressReporter() = default;
    virtual void update(ObserverMessage message, const std::string& value) const override;
};