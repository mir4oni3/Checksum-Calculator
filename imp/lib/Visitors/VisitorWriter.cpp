#include "Visitors/VisitorWriter.hpp"
#include "Files/FileIterator.hpp"
#include "Observers/ObserverMessage.hpp"

VisitorWriter::VisitorWriter(std::ostream& os) : os(os) {
    if (!os) {
        throw std::invalid_argument("VisitorWriter::VisitorWriter - invalid ostream");
    }
}

void VisitorWriter::exportFile(File& file) const {
    notifyObservers(ObserverMessage::rootFileSize, std::to_string(file.getSize()));

    FileIterator it(file);

    setupExport();
    while (it.hasNext()) {
        const File& next = it.next();
        this->currentFilePath = next.getPath();
        next.accept(*this);
    }
    finalizeExport();
}

void VisitorWriter::visitRegularFile(const RegularFile& file) const {
    notifyObservers(ObserverMessage::newRegularFile, file.getPath());
}

void VisitorWriter::visitDirectory(const Directory& dir) const {
    notifyObservers(ObserverMessage::newDirectory, dir.getPath());
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