#pragma once

#include "Observers/Observable.hpp"

#include <fstream>
#include <functional>

namespace CalcConstants {
    const size_t chunkSize = 10000;// read 10 000 bytes at a time
    const size_t waitTime = 100; // wait 100 ms before checking if calculation is unpaused
}

class ChecksumCalculator : public Observable, public Observer {
protected:
    bool pause = false;
    void wait() const;
    
public:
    virtual ~ChecksumCalculator() = default;
    virtual std::string calculate(std::istream&) const = 0;
    void update(ObserverMessage message, const std::string& value) override;
};