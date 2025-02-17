#include "Files/FileSystemBuilder.hpp"
#include "Factories/CalculatorFactory.hpp"

#include <filesystem>
#include <memory>
#include <iostream>
#include <unordered_set>
#include <set>

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

static std::set<std::string> getSortedSubFiles(const std::string& path) {
    std::set<std::string> files;
    for (const auto& entry : fs::directory_iterator(path)) {
        files.insert(entry.path().string());
    }
    return files;
}

static std::unique_ptr<File> buildCommon(const std::string& path, bool traverse, const std::unique_ptr<ChecksumCalculator>& calc) {
    std::string curPath = path;

    if (traverse) {
        checkSymlink(curPath);
    }

    if (fs::is_regular_file(curPath)) {
        std::unique_ptr<RegularFile> file = std::make_unique<RegularFile>(curPath);
        file->setSize(fs::file_size(curPath));
        if (calc) {
            //checksum is calculated and stored on first getChecksum() call due to lazy init implementation
            file->getChecksum(*calc);
        }
        return std::move(file);
    }

    if (fs::is_directory(curPath)) {
        std::unique_ptr<Directory> dir = std::make_unique<Directory>(curPath);
        dir->setSize(0);

        std::set<std::string> subfiles = getSortedSubFiles(curPath);
        for (const std::string& entry : subfiles) {
            std::unique_ptr<File> file = buildCommon(entry, traverse, calc);
            if (!file) {
                //we will store only regular files and directories
                continue;
            }
            dir->setSize(dir->getSize() + file->getSize());
            dir->addFile(std::move(file));
        }
        return std::move(dir);
    }

    return nullptr;
}

//setting the algorithm means that the checksums will be calculated during the file building process
//default behavior is to build without calculating checksums
void FileSystemBuilder::setAlgorithm(const std::string& algorithm) {
    this->algorithm = algorithm;
}

void FileSystemBuilder::saveFullPaths(bool save) {
    this->saveFullPath = save;
}

std::unique_ptr<File> IgnoreSymlinkFileSystemBuilder::build(const std::string& path) const {
    if (!fs::exists(path)) {
        throw std::invalid_argument("TraverseSymlinkFileSystemBuilder::build - Path does not exist");
    }

    std::unique_ptr<ChecksumCalculator> calculator = nullptr;
    if (this->algorithm != "") {
        //user specified to calculate the checksums in the file building process
        calculator = ChecksumCalculatorFactory::getCalculator(this->algorithm);
    }

    std::string actualPath = path;
    if (this->saveFullPath) {
        actualPath = fs::absolute(path).string();
    }

    return buildCommon(actualPath, false, calculator);
}

//NOTE: this implementation will traverse UNIX symlinks only
//Windows .lnk files(or any other files) won't be treated as symlinks
std::unique_ptr<File> TraverseSymlinkFileSystemBuilder::build(const std::string& path) const {
    if (!fs::exists(path)) {
        throw std::invalid_argument("TraverseSymlinkFileSystemBuilder::build - Path does not exist");
    }

    std::unique_ptr<ChecksumCalculator> calculator = nullptr;
    if (this->algorithm != "") {
        //user specified to calculate the checksums in the file building process
        calculator = ChecksumCalculatorFactory::getCalculator(this->algorithm);
    }

    std::string actualPath = path;
    if (this->saveFullPath) {
        actualPath = fs::absolute(path).string();
    }

    return buildCommon(actualPath, true, calculator);
}