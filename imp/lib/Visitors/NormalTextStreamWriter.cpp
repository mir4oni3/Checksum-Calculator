#include "Visitors/NormalTextStreamWriter.hpp"

#include <regex>
#include <sstream>

NormalTextStreamWriter::NormalTextStreamWriter(std::ostream& os, const std::shared_ptr<ChecksumCalculator>& calc) : HashStreamWriter(os, calc) {}

void NormalTextStreamWriter::visitRegularFile(const RegularFile& file) const {
    VisitorWriter::visitRegularFile(file); //notify observers
    tempStream << file.getChecksum(calc) << " *" << file.getPath() << "\n";
}