#pragma once

#include "Visitors/HashStreamWriter.hpp"

class HashStreamWriterFactory {
public:
    static std::unique_ptr<HashStreamWriter> getWriter(const std::string& format, std::ostream& os, ChecksumCalculator& calc);
};