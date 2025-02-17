#include "Visitors/NormalTextStreamWriter.hpp"

#include <regex>
#include <sstream>

NormalTextStreamWriter::NormalTextStreamWriter(std::ostream& os, ChecksumCalculator& calc) : HashStreamWriter(os, calc) {}

void NormalTextStreamWriter::visitRegularFile(const RegularFile& file) const {
    VisitorWriter::visitRegularFile(file); //notify observers
    tempStream << file.getChecksum(calc) << " *" << file.getPath() << "\n";
}