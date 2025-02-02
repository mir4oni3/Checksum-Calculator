#include "Visitors/VisitorWriter.hpp"
#include "Files/FileIterator.hpp"
#include "Observers/ObserverMessage.hpp"

VisitorWriter::VisitorWriter(std::ostream& os) : os(os) {
    if (!os) {
        throw std::invalid_argument("VisitorWriter::VisitorWriter - invalid ostream");
    }
}

void VisitorWriter::exportFile(const std::shared_ptr<File>& file) const {
    if (!file) {
        throw std::invalid_argument("VisitorWriter::export - invalid file ptr");
    }

    notifyObservers(ObserverMessage::rootFileSize, std::to_string(file->getSize()));

    FileIterator it(file);

    setupExport();
    while (this->currentFile = it.next()) {
        currentFile->accept(*this);
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