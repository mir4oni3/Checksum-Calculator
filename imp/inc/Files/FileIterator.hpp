#pragma once

#include "File.hpp"

#include <stack>

class FileIterator {
protected:
    std::stack<std::pair<std::shared_ptr<File>, size_t>> stack;
public:
    FileIterator(const std::shared_ptr<File>&);
    virtual ~FileIterator() = default;

    virtual std::shared_ptr<File> next();
};