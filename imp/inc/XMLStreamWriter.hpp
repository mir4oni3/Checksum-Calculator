#include "HashStreamWriter.hpp"

class XMLStreamWriter : public HashStreamWriter {
public:
    XMLStreamWriter(std::ostream& os, const std::shared_ptr<ChecksumCalculator>& calc);
    
    void visitRegularFile(const RegularFile& file) const override;

    virtual void setupExport() const override;
    virtual void finalizeExport() const override;

    void parseLine(const std::string& line, std::unordered_map<std::string, std::string>& files) const override;
};