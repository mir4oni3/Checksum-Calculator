#pragma once

#include "Files/File.hpp"
#include "Observers/Observable.hpp"

#include <memory>
#include <ostream>

class VisitorWriter : public Observable {
protected:
    std::ostream& os;

public:
    VisitorWriter(std::ostream& os);
    virtual ~VisitorWriter() = default;

    //write file(and subfiles recursively) and its checksum to a stream
    void exportFile(const std::shared_ptr<File>&) const;

    virtual void visitRegularFile(const RegularFile&) const;
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