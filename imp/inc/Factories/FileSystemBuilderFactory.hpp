#pragma once

#include "Files/FileSystemBuilder.hpp"

class FileSystemBuilderFactory {
public:
    static std::unique_ptr<FileSystemBuilder> getBuilder(bool traverse);
};