#include "Factories/HashStreamWriterFactory.hpp"
#include "Visitors/NormalTextStreamWriter.hpp"
#include "Visitors/XMLStreamWriter.hpp"

std::unique_ptr<HashStreamWriter> HashStreamWriterFactory::getWriter(const std::string& format,  std::ostream& os, ChecksumCalculator& calc) {
    if (format == "text") {
        return std::make_unique<NormalTextStreamWriter>(os, calc);
    }
    if (format == "xml") {
        return std::make_unique<XMLStreamWriter>(os, calc);
    }
    throw std::invalid_argument("HashStreamWriterFactory::getWriter - Unsupported format");
    return nullptr;
}