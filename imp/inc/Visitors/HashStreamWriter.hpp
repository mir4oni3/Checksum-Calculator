#pragma once

#include "VisitorWriter.hpp"

#include <thread>
#include <atomic>
#include <sstream>

class HashStreamWriter : public VisitorWriter {
protected:
    ChecksumCalculator& calc;
    mutable std::ostringstream tempStream;

public:
    HashStreamWriter(std::ostream& os, ChecksumCalculator& calc);
    virtual ~HashStreamWriter() = default;

    virtual void visitRegularFile(const RegularFile&) const override = 0;

    virtual void addObserver(Observer& observer) override;

    virtual void setupExport() const override;
    virtual void finalizeExport() const override;

protected:
    virtual void pauseListener() const;
};