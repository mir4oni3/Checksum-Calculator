#pragma once

#include "Observers/Observable.hpp"

#include <fstream>
#include <functional>

namespace CalcConstants {
    const size_t chunkSize = 1024; // read 1 KB at a time
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