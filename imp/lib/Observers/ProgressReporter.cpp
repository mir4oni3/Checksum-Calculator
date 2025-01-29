#include "Observers/ProgressReporter.hpp"
#include "Observers/ObserverMessage.hpp"

#include <ostream>

ProgressReporter::ProgressReporter(std::ostream& reportTo) : os(reportTo) {
    if (!os) {
        throw std::invalid_argument("ProgressReporter::ProgressReporter - invalid ostream");
    }
}

void ProgressReporter::update(ObserverMessage message, const std::string& value) {
    switch(message) {
        case ObserverMessage::rootFileSize:
            totalBytes = std::stoull(value);
            totalBytesKnown = true;
            break;
        case ObserverMessage::progress:
            currentBytesRead += std::stoull(value);
            visualizeProgressBar();
            break;
        case ObserverMessage::newRegularFile:
            os << "\r" << "\033[F" << "\033[F" << "\033[2K" << "\r"; //setup cursor
            os << "Processing " << value << "...\n";
            startTime = timerStarted ? startTime : std::chrono::system_clock::now();
            timerStarted = true;
            break;
        case ObserverMessage::allFilesHandled:
            totalBytes = 0;
            totalBytesKnown = false;
            currentBytesRead = 0;
            timerStarted = false;
            break;
    }
}

void ProgressReporter::visualizeProgressBar() const {
    if (!totalBytesKnown) {
        throw std::runtime_error("ProgressReporter::visualizeProgressBar - Total size unknown");
    }

    double currentProgress = static_cast<double>(this->currentBytesRead) / this->totalBytes;
    size_t fillCount = static_cast<size_t>(currentProgress * ReporterConstants::progressBarWidth);
    size_t percentage = static_cast<size_t>(currentProgress * 100);

    os << "\r" << "\033[2K" << "\r"; //clear line

    os << "[";
    for (size_t i = 0; i < ReporterConstants::progressBarWidth; ++i) {
        if (i < fillCount) {
            os << ReporterConstants::progressChar;
        } else {
            os << ReporterConstants::remainingChar;
        }
    }

    //get elapsed time
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - startTime);
    
    size_t remaining = elapsed.count() / currentProgress - elapsed.count(); //expected remaining time

    os << "] " << currentBytesRead << "B / " << totalBytes << "B";
    os <<  ", " << percentage << "% ready, remaining time: " << remaining << "s";
    os.flush();
}