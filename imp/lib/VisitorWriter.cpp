#include "../inc/VisitorWriter.hpp"
#include "../inc/FileIterator.hpp"
#include "../inc/ObserverMessages.hpp"

VisitorWriter::VisitorWriter(std::ostream& os) : os(os) {}

void VisitorWriter::exportFile(const std::shared_ptr<File>& file) const {
    if (!file) {
        throw std::invalid_argument("VisitorWriter::export - invalid file ptr");
    }

    FileIterator it(file);

    setupExport();
    while (it.hasMore()) {
        std::shared_ptr<File> current = it.next();
        current->accept(*this);
    }
    finalizeExport();
}

void VisitorWriter::visitRegularFile(const RegularFile& file) const {
    notifyObservers(ObserverMessages::newRegularFile, file.getPath());
}

void VisitorWriter::visitDirectory(const Directory& dir) const {
    notifyObservers(ObserverMessages::newDirectory, dir.getPath());
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