#include "Visitors/HashStreamWriter.hpp"

#include <iostream>
#include <thread>

HashStreamWriter::HashStreamWriter(std::ostream& os, ChecksumCalculator& calc) : VisitorWriter(os), calc(calc) {
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
            notifyObservers(ObserverMessage::resumeCalculation, this->currentFilePath);
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
void HashStreamWriter::addObserver(Observer& observer) {
    VisitorWriter::addObserver(observer);
    calc.addObserver(observer);
}