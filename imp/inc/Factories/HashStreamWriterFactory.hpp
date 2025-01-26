#pragma once

#include "Visitors/HashStreamWriter.hpp"

class HashStreamWriterFactory {
public:
    static std::shared_ptr<HashStreamWriter> getWriter(const std::string& format, std::ostream& os, const std::shared_ptr<ChecksumCalculator>& calc);
};