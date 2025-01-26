#include "../inc/VisitorWriter.hpp"
#include "../inc/FileIterator.hpp"

VisitorWriter::VisitorWriter(std::ostream& os) : os(os) {}

void VisitorWriter::export(const std::shared_ptr<File>& file) const {
    if (!file) {
        throw std::invalid_argument("VisitorWriter::export - invalid file ptr");
    }

    FileIterator it(file);
    while (it.hasMore()) {
        std::shared_ptr<File> current = it.next();
        current->accept(*this);
    }
}

void VisitorWriter::visitDirectory(const Directory& dir) const {
    //do nothing by default
}

void VisitorWriter::setupExport() const {
    //do nothing by default
}

void VisitorWriter::finalizeExport() const {
    //do nothing by default
}

ReportWriter::ReportWriter(std::ostream& os) : VisitorWriter(os) {}

void ReportWriter::visitRegularFile(const RegularFile& file) const {
    os << "File: " << file.getPath() << " , Size: " << file.getSize() << "B" << std::endl;
}