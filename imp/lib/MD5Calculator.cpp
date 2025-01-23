#include "../inc/MD5Calculator.hpp"
#include "../../resources/librhash/md5.h"

#include <sstream>
#include <iomanip>

std::string MD5Calculator::calculate(std::istream& is) const {
    if (!is) {
        throw std::invalid_argument("MD5Calculator::calculate - Invalid input stream state");
    }

    md5_ctx context;
    rhash_md5_init(&context);

    //read data in chunks
    char buffer[CalcConstants::chunkSize];
    while (is.read(buffer, CalcConstants::chunkSize)) {
        rhash_md5_update(&context, reinterpret_cast<const unsigned char*>(buffer), CalcConstants::chunkSize);
    }

    if (is.bad()) {
        throw std::runtime_error("MD5Calculator::calculate - Error reading from input stream");
    }

    //read remaining data, which is less than 1 chunk
    size_t bytesRead = is.gcount();
    if (bytesRead > 0) {
        rhash_md5_update(&context, reinterpret_cast<const unsigned char*>(buffer), bytesRead);
    }

    unsigned char result[md5_hash_size];
    rhash_md5_final(&context, result);

    // Convert the result to a hex string
    std::ostringstream hexResult;
    hexResult << std::hex << std::setfill('0');
    for (int i = 0; i < md5_hash_size; ++i) {
        hexResult << std::setw(2) << static_cast<int>(result[i]);
    }

    //return original state of stream
    is.clear();
    is.seekg(0, std::ios::beg);
    
    return hexResult.str();
}