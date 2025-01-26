#include "../inc/ProgressReporter.hpp"
#include "../inc/ObserverMessages.hpp"

#include <ostream>

ProgressReporter::ProgressReporter(std::ostream& reportTo) : os(reportTo) {
    if (!os) {
        throw std::invalid_argument("ProgressReporter::ProgressReporter - invalid ostream");
    }
}

void ProgressReporter::update(const std::string& message, const std::string& value) const {
    if (message == ObserverMessages::newRegularFile) {
        os << "Processing " << value << "...\n";
    }
}