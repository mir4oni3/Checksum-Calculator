#include "Factories/FileSystemBuilderFactory.hpp"

std::unique_ptr<FileSystemBuilder> FileSystemBuilderFactory::getBuilder(bool traverse) {
    if (traverse) {
        return std::make_unique<TraverseSymlinkFileSystemBuilder>();
    }
    else {
        return std::make_unique<FileSystemBuilder>();
    }
    return nullptr;
}