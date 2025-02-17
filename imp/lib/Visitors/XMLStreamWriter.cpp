#include "Visitors/XMLStreamWriter.hpp"

#include <regex>

XMLStreamWriter::XMLStreamWriter(std::ostream& os, ChecksumCalculator& calc) : HashStreamWriter(os, calc) {}

void XMLStreamWriter::visitRegularFile(const RegularFile& file) const {
        VisitorWriter::visitRegularFile(file); //notify observers

        tempStream << "    <item>\n";
        tempStream << "        <mode>binary</mode>\n";
        tempStream << "        <checksum>" << file.getChecksum(calc) << "</checksum>\n";
        tempStream << "        <path>" << file.getPath() << "</path>\n";
        tempStream << "        <size>" << file.getSize() << "</size>\n";
        tempStream << "    </item>\n";
}

void XMLStreamWriter::setupExport() const {
        tempStream << "<checksums>\n";
}

void XMLStreamWriter::finalizeExport() const {
        HashStreamWriter::finalizeExport();
        os << "</checksums>\n";
}