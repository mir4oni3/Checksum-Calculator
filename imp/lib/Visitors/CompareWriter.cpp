#include "Visitors/CompareWriter.hpp"

#include <iostream>

CompareWriter::CompareWriter(std::ostream& os, ChecksumCalculator& calc, std::unordered_map<std::string, std::string>& checksums) : HashStreamWriter(os, calc), checksums(checksums) {}

void CompareWriter::visitRegularFile(const RegularFile& regFile) const {
    VisitorWriter::visitRegularFile(regFile); //notify observers

    std::string currentPath = regFile.getPath();
    std::string checksum = regFile.getChecksum(calc);

    if (checksums.find(currentPath) == checksums.end()) {
        tempStream << currentPath << ": NEW\n";
        return;
    }
    else if (!calc.verifyChecksum(checksums.at(currentPath))) {
        tempStream << currentPath << ": INVALID\n";
        std::cerr << "WARNING: Checksum for " << currentPath << " is not in the right format.\n";
    }
    else if (checksum != checksums.at(currentPath)) {
        tempStream << currentPath << ": MODIFIED\n";
    }
    else if (checksum == checksums.at(currentPath)) {
        tempStream << currentPath << ": OK\n";
    }

    checksums.erase(currentPath);
}

void CompareWriter::finalizeExport() const {
    HashStreamWriter::finalizeExport();

    for (const auto& [path, checksum] : checksums) {
        tempStream << path << ": REMOVED\n";
    }
}