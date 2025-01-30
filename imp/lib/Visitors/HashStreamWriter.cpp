#include "Visitors/HashStreamWriter.hpp"

#include <iostream>
#include <thread>

HashStreamWriter::HashStreamWriter(std::ostream& os, const std::shared_ptr<ChecksumCalculator>& calc) : VisitorWriter(os), calc(calc) {
    if (!calc) {
        throw std::invalid_argument("HashStreamWriter::HashStreamWriter - Invalid calculator passed");
    }
    tempStream << "\n\n";
    addObserver(calc);
}

void HashStreamWriter::setupExport() const {
    std::cout << "\033[2J\033[1;1H"; //clear screen
    
    //run pauseListener on another thread
    std::thread(&HashStreamWriter::pauseListener, this).detach();
}

void HashStreamWriter::finalizeExport() const {
    os << tempStream.str();
    tempStream.str("");
    notifyObservers(ObserverMessage::allFilesHandled, "");
}

void HashStreamWriter::pauseListener() const {
    std::string dummy;
    bool paused = false;
    while (true) {
        std::cin >> dummy; //wait for input from stdin(blocking the thread)
        if (paused) {
            std::cout << "\033[2J\033[1;1H"; //clear screen
            notifyObservers(ObserverMessage::resumeCalculation, this->currentFile->getPath());
        }
        else {
            notifyObservers(ObserverMessage::pauseCalculation, "");
            std::cout << "\r" << "\033[2K" << "\r"; //clear line
            std::cout << "Paused. Press any key to resume.\n";
        }
        paused = !paused;
    }
}

//the Observer should receive messages from calculator as well
void HashStreamWriter::addObserver(const std::shared_ptr<Observer>& observer) {
    VisitorWriter::addObserver(observer);
    calc->addObserver(observer);
}