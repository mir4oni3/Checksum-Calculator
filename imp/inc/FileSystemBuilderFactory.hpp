#pragma once

#include "FileSystemBuilder.hpp"

class FileSystemBuilderFactory {
public:
    static std::shared_ptr<FileSystemBuilder> getBuilder(bool traverse);
};