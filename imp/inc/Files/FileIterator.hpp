#pragma once

#include "File.hpp"

#include <stack>

class FileIterator {
protected:
    std::stack<std::pair<const File&, size_t>> stack;
public:
    FileIterator(const File&);
    virtual ~FileIterator() = default;

    bool hasNext() const;
    virtual const File& next();
};