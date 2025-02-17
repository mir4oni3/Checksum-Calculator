#include "Files/FileSystemBuilder.hpp"
#include "Factories/CalculatorFactory.hpp"

#include <filesystem>
#include <memory>
#include <iostream>
#include <set>

namespace fs = std::filesystem;

std::unique_ptr<File> FileSystemBuilder::handleDirectory(const std::string& path, const std::unique_ptr<ChecksumCalculator>& calc) const {
    std::unique_ptr<Directory> dir = std::make_unique<Directory>(path);
    dir->setSize(0);

    for (const auto& entry : fs::directory_iterator(path)) {
        std::unique_ptr<File> file = buildCommon(entry.path().string(), calc);
        if (!file) {
            //we will store only regular files and directories
            continue;
        }
        dir->setSize(dir->getSize() + file->getSize());
        dir->addFile(std::move(file));
    }

    if (this->sortFiles) {
        dir->sortFiles();
    }

    return std::move(dir);
}

std::unique_ptr<File> FileSystemBuilder::buildCommon(const std::string& path, const std::unique_ptr<ChecksumCalculator>& calc) const {
    if (fs::is_regular_file(path)) {
        std::unique_ptr<RegularFile> file = std::make_unique<RegularFile>(path);
        file->setSize(fs::file_size(path));
        if (calc) {
            //checksum is calculated and stored on first getChecksum() call due to lazy init implementation
            file->getChecksum(*calc);
        }
        return std::move(file);
    }

    if (fs::is_directory(path)) {
       return std::move(handleDirectory(path, calc));
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

void FileSystemBuilder::setSortFiles(bool sort) {
    this->sortFiles = sort;
}

std::unique_ptr<File> FileSystemBuilder::build(const std::string& path) const {
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

    return std::move(buildCommon(actualPath, calculator));
}

//symlinks will be treated as regular files in these scenarios:
//  -symlink points to a visited file
//  -symlink is broken
std::unique_ptr<File> TraverseSymlinkFileSystemBuilder::buildCommon(const std::string& path, const std::unique_ptr<ChecksumCalculator>& calc) const {
    visited.insert(path);
    std::string curPath = path;

    if (fs::is_symlink(path)) {
        try {
            curPath = fs::read_symlink(path).string();
            if (visited.find(curPath) != visited.end()) {
                //symlink points to visited file; treat as regular file
                return std::move(FileSystemBuilder::buildCommon(path, calc));
            }
        } catch (const fs::filesystem_error& e) {
            //broken symlink, treat as regular file
            return std::move(FileSystemBuilder::buildCommon(path, calc));
        }
    }

    if (fs::is_symlink(curPath)) {
        //symlink points to symlink, traverse
        return std::move(TraverseSymlinkFileSystemBuilder::buildCommon(curPath, calc));
    }
    else {
        //symlink points to regular file or directory
        return std::move(FileSystemBuilder::buildCommon(curPath, calc));
    }

    return nullptr;
}