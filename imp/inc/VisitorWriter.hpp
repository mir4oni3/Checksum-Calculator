#pragma once

#include "File.hpp"

#include <memory>
#include <ostream>


class VisitorWriter {
protected:
    std::ostream& os;

public:
    VisitorWriter(std::ostream& os);
    virtual ~VisitorWriter() = default;

    //write file(and subfiles recursively) and its checksum to a stream
    void export(const std::shared_ptr<File>&) const;

    virtual void visitRegularFile(const RegularFile&) const = 0;
    virtual void visitDirectory(const Directory&) const;

protected:
    virtual void setupExport() const;
    virtual void finalizeExport() const;
};

class ReportWriter : public VisitorWriter {
public:
    ReportWriter(std::ostream& os);

    void visitRegularFile(const RegularFile& file) const override;
};