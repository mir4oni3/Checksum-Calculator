#pragma once

#include "HashStreamWriter.hpp"

class HashStreamWriterFactory {
public:
    static std::shared_ptr<HashStreamWriter> getWriter(const std::string& format, const std::shared_ptr<ChecksumCalculator>& calc, std::ostream& os);
};