#include "../inc/FileSystemBuilder.hpp"

#include <filesystem>
#include <memory>
#include <iostream>
#include <unordered_set>

namespace fs = std::filesystem;

//symlinks will be treated as regular files in these scenarios:
//  -symlink points to a visited file
//  -symlink is broken
static void checkSymlink(std::string& curPath) {
    static std::unordered_set<std::string> visited;
    visited.insert(curPath);

    if (fs::is_symlink(curPath)) {
        std::string pointingTo;
        try {
            pointingTo = fs::read_symlink(curPath).string();
        } catch (const fs::filesystem_error& e) {
            return;
        }
        if (!visited.count(pointingTo)) {
            curPath = pointingTo;
            visited.insert(curPath);
        }
    }
}

static std::shared_ptr<File> buildCommon(const std::string& path, bool traverse) {
    std::string curPath = path;

    if (traverse) {
        checkSymlink(curPath);
    }

    if (fs::is_regular_file(curPath)) {
        std::shared_ptr<File> file = std::make_shared<RegularFile>(curPath);
        file->setSize(fs::file_size(curPath));
        return std::make_shared<RegularFile>(curPath);
    }

    if (fs::is_directory(curPath)) {
        std::shared_ptr<Directory> dir = std::make_shared<Directory>(curPath);
        for (const auto& entry : fs::directory_iterator(curPath)) {
            std::shared_ptr<File> file = buildCommon(entry.path().string(), traverse);
            if (!file) {
                //we will store only regular files and directories
                continue;
            }
            dir->addFile(file);
            dir->setSize(dir->getSize() + file->getSize());
        }
        return dir;
    }

    return nullptr;
}

std::shared_ptr<File> IgnoreSymlinkFileSystemBuilder::build(const std::string& path) const {
    return buildCommon(path, false);
}

//NOTE: this implementation will traverse UNIX symlinks only
//Windows .lnk files(or any other files) won't be treated as symlinks
std::shared_ptr<File> TraverseSymlinkFileSystemBuilder::build(const std::string& path) const {
    return buildCommon(path, true);
}