#pragma once

#include "HashStreamWriter.hpp"

class NormalTextStreamWriter : public HashStreamWriter {
public:
    NormalTextStreamWriter(std::ostream& os, const std::shared_ptr<ChecksumCalculator>& calc);
    
    void visitRegularFile(const RegularFile& file) const override;
};
