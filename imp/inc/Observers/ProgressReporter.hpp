#pragma once

#include "Observer.hpp"
#include "ObserverMessage.hpp"

#include <chrono>

namespace ReporterConstants {
    const uint8_t progressBarWidth = 30;
    const char progressChar = '#';
    const char remainingChar = '.';
}

class ProgressReporter : public Observer {
protected:
    std::ostream& os;
    size_t totalBytes = 0;
    bool totalBytesKnown = false;
    size_t currentBytesRead = 0;
    std::chrono::time_point<std::chrono::system_clock> startTime;
    bool timerStarted = false;
    
public:
    ProgressReporter(std::ostream& reportTo);
    virtual ~ProgressReporter() = default;

    virtual void update(ObserverMessage message, const std::string& value) override;
    void visualizeProgressBar() const;
};