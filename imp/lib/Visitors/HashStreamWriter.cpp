#include "Visitors/HashStreamWriter.hpp"

HashStreamWriter::HashStreamWriter(std::ostream& os, const std::shared_ptr<ChecksumCalculator>& calc) : VisitorWriter(os), calc(calc) {
    if (!calc) {
        throw std::invalid_argument("HashStreamWriter::HashStreamWriter - Invalid calculator passed");
    }
    tempStream << "\n\n";
}

void HashStreamWriter::finalizeExport() const {
    os << tempStream.str();
    tempStream.str("");
    notifyObservers(ObserverMessage::allFilesHandled, "true");
}

//the Observer should receive messages from calculator as well
void HashStreamWriter::addObserver(const std::shared_ptr<Observer>& observer) {
    VisitorWriter::addObserver(observer);
    calc->addObserver(observer);
}