#pragma once

#include "Visitors/HashStreamWriter.hpp"

class CompareWriter : public HashStreamWriter {
protected:
    std::unordered_map<std::string, std::string>& checksums;
public:
    CompareWriter(std::ostream& os, ChecksumCalculator& calc, std::unordered_map<std::string, std::string>& checksums);
    void visitRegularFile(const RegularFile& file) const override;
    void finalizeExport() const override;
};