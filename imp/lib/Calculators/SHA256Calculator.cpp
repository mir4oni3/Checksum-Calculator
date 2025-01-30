#include "Calculators/SHA256Calculator.hpp"
#include "librhash/sha256.h"

#include <sstream>
#include <iomanip>

std::string SHA256Calculator::calculate(std::istream& is) const {
    if (!is) {
        throw std::invalid_argument("SHA256Calculator::calculate - Invalid input stream state");
    }

    wait();

    sha256_ctx context;
    rhash_sha256_init(&context);

    //read data in chunks
    char buffer[CalcConstants::chunkSize];
    while (is.read(buffer, CalcConstants::chunkSize)) {
        wait();
        rhash_sha256_update(&context, reinterpret_cast<const unsigned char*>(buffer), CalcConstants::chunkSize);
        notifyObservers(ObserverMessage::progress, std::to_string(CalcConstants::chunkSize));
    }

    if (is.bad()) {
        throw std::runtime_error("SHA256Calculator::calculate - Error reading from input stream");
    }

    //read remaining data, which is less than 1 chunk
    size_t bytesRead = is.gcount();
    if (bytesRead > 0) {
        rhash_sha256_update(&context, reinterpret_cast<const unsigned char*>(buffer), bytesRead);
        notifyObservers(ObserverMessage::progress, std::to_string(bytesRead));
    }

    wait();

    unsigned char result[sha256_hash_size];
    rhash_sha256_final(&context, result);

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