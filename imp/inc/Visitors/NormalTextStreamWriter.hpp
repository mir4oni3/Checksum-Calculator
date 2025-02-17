#pragma once

#include "HashStreamWriter.hpp"

class NormalTextStreamWriter : public HashStreamWriter {
public:
    NormalTextStreamWriter(std::ostream& os, ChecksumCalculator& calc);
    
    void visitRegularFile(const RegularFile& file) const override;
};
