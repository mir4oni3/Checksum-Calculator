#pragma once

#include "File.hpp"

#include <memory>
#include <ostream>


class VisitorWriter {
protected:
    std::ostream& os;
    const std::shared_ptr<ChecksumCalculator> calc;

public:
    VisitorWriter(std::ostream& os, const std::shared_ptr<ChecksumCalculator>& calc);
    virtual ~VisitorWriter() = default;

    //write file(and subfiles recursively) and its checksum to a stream
    void export(const std::shared_ptr<File>&) const;

    virtual void visitRegularFile(const RegularFile&) const = 0;
    virtual void visitDirectory(const Directory&) const = 0;
};