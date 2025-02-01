#include "Calculators/ChecksumCalculator.hpp"

#include <thread>

void ChecksumCalculator::update(ObserverMessage message, const std::string& value) {
    if (message == ObserverMessage::resumeCalculation) {
        this->pause = false;
    }
    if (message == ObserverMessage::pauseCalculation) {
        this->pause = true;
    }
}

//wait until calculation is unpaused
void ChecksumCalculator::wait() const {
    while (this->pause) {
        //wait before checking again to prevent busy waiting (reduce CPU load)
        std::this_thread::sleep_for(std::chrono::milliseconds(CalcConstants::waitTime));
    }
}