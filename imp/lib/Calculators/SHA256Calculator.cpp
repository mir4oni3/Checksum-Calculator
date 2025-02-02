#include "Calculators/SHA256Calculator.hpp"
#include "librhash/sha256.h"

#include <sstream>
#include <iomanip>

std::string SHA256Calculator::calculate(std::istream& is) const {
    if (!is) {
        throw std::invalid_argument("SHA256Calculator::calculate - Invalid input stream state");
    }

    sha256_ctx context;
    sha256_ctx* contextPtr = &context;

    static std::string chunkSizeStr = std::to_string(CalcConstants::chunkSize);

    rhash_sha256_init(contextPtr);

    //read data in chunks
    char buffer[CalcConstants::chunkSize];
    const unsigned char* data = reinterpret_cast<const unsigned char*>(buffer);

    while (is.read(buffer, CalcConstants::chunkSize)) {
        wait(); //check if calculation is paused
        rhash_sha256_update(contextPtr, data, CalcConstants::chunkSize);
        notifyObservers(ObserverMessage::progress, chunkSizeStr);
    }

    if (is.bad()) {
        throw std::runtime_error("SHA256Calculator::calculate - Error reading from input stream");
    }

    //read remaining data, which is less than 1 chunk
    size_t bytesRead = is.gcount();
    if (bytesRead > 0) {
        rhash_sha256_update(contextPtr, data, bytesRead);
        notifyObservers(ObserverMessage::progress, std::to_string(bytesRead));
    }

    unsigned char result[sha256_hash_size];
    rhash_sha256_final(contextPtr, result);

    // Convert the result to a hex string
    std::ostringstream hexResult;
    hexResult << std::hex << std::setfill('0');
    for (int i = 0; i < sha256_hash_size; ++i) {
        hexResult << std::setw(2) << static_cast<int>(result[i]);
    }

    //return original state of stream
    is.clear();
    is.seekg(0, std::ios::beg);
    is.clear();

    return hexResult.str();
}