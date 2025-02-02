#include "Factories/HashStreamWriterFactory.hpp"
#include "Visitors/NormalTextStreamWriter.hpp"
#include "Visitors/XMLStreamWriter.hpp"

std::shared_ptr<HashStreamWriter> HashStreamWriterFactory::getWriter(const std::string& format,  std::ostream& os, const std::shared_ptr<ChecksumCalculator>& calc) {
    if (format == "text") {
        return std::make_shared<NormalTextStreamWriter>(os, calc);
    }
    if (format == "xml") {
        return std::make_shared<XMLStreamWriter>(os, calc);
    }
    throw std::invalid_argument("HashStreamWriterFactory::getWriter - Unsupported format");
    return nullptr;
}