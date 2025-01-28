#include "Observers/ProgressReporter.hpp"
#include "Observers/ObserverMessage.hpp"

#include <ostream>

ProgressReporter::ProgressReporter(std::ostream& reportTo) : os(reportTo) {
    if (!os) {
        throw std::invalid_argument("ProgressReporter::ProgressReporter - invalid ostream");
    }
}

void ProgressReporter::update(ObserverMessage message, const std::string& value) const {
    if (message == ObserverMessage::rootFileSize) {
        totalSize = std::stoull(value);
        isTotalSizeSet = true;
    }
    if (message == ObserverMessage::newRegularFile) {
        os << "Processing " << value << "...\n";
    }
}