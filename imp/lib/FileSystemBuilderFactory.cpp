#include "../inc/FileSystemBuilderFactory.hpp"

std::shared_ptr<FileSystemBuilder> FileSystemBuilderFactory::getBuilder(bool traverse) {
    if (traverse) {
        return std::make_shared<TraverseSymlinkFileSystemBuilder>();
    }
    else {
        return std::make_shared<IgnoreSymlinkFileSystemBuilder>();
    }
    return nullptr;
}