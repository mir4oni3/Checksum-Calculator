#pragma once

#include "Files/FileSystemBuilder.hpp"

class FileSystemBuilderFactory {
public:
    static std::shared_ptr<FileSystemBuilder> getBuilder(bool traverse);
};