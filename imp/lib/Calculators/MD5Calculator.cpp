#include "Calculators/MD5Calculator.hpp"
#include "librhash/md5.h"

#include <sstream>
#include <iomanip>

const std::string MD5Calculator::md5Regex = "[0-9a-fA-F]{32}";

std::string MD5Calculator::calculate(std::istream& is) const {
    if (!is) {
        throw std::invalid_argument("MD5Calculator::calculate - Invalid input stream state");
    }

    md5_ctx context;
    md5_ctx* contextPtr = &context;

    static std::string chunkSizeStr = std::to_string(CalcConstants::chunkSize);

    rhash_md5_init(contextPtr);

    //read data in chunks
    char buffer[CalcConstants::chunkSize];
    const unsigned char* data = reinterpret_cast<const unsigned char*>(buffer);

    while (is.read(buffer, CalcConstants::chunkSize)) {
        wait(); //check if calculation is paused
        rhash_md5_update(contextPtr, data, CalcConstants::chunkSize);
        notifyObservers(ObserverMessage::progress, chunkSizeStr);
    }

    if (is.bad()) {
        throw std::runtime_error("MD5Calculator::calculate - Error reading from input stream");
    }

    //read remaining data, which is less than 1 chunk
    size_t bytesRead = is.gcount();
    if (bytesRead > 0) {
        rhash_md5_update(contextPtr, data, bytesRead);
        notifyObservers(ObserverMessage::progress, std::to_string(bytesRead));
    }

    unsigned char result[md5_hash_size];
    rhash_md5_final(contextPtr, result);

    // Convert the result to a hex string
    std::ostringstream hexResult;
    hexResult << std::hex << std::setfill('0');
    for (int i = 0; i < md5_hash_size; ++i) {
        hexResult << std::setw(2) << static_cast<int>(result[i]);
    }

    //return original state of stream
    is.clear();
    is.seekg(0, std::ios::beg);
    is.clear();
    
    return hexResult.str();
}

std::regex MD5Calculator::getRegex() const {
    static const std::regex reg(MD5Calculator::md5Regex);
    return reg;
}