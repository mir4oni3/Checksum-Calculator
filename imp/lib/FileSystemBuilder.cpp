#include "../inc/FileSystemBuilder.hpp"
#include "../inc/CalculatorFactory.hpp"

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

static std::shared_ptr<File> buildCommon(const std::string& path, bool traverse, const std::shared_ptr<ChecksumCalculator>& calc) {
    std::string curPath = path;

    if (traverse) {
        checkSymlink(curPath);
    }

    if (fs::is_regular_file(curPath)) {
        std::shared_ptr<RegularFile> file = std::make_shared<RegularFile>(curPath);
        file->setSize(fs::file_size(curPath));
        if (calc) {
            //checksum is calculated and stored on first getChecksum() call due to lazy init implementation
            file->getChecksum(calc);
        }
        return file;
    }

    if (fs::is_directory(curPath)) {
        std::shared_ptr<Directory> dir = std::make_shared<Directory>(curPath);
        for (const auto& entry : fs::directory_iterator(curPath)) {
            std::shared_ptr<File> file = buildCommon(entry.path().string(), traverse, calc);
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

//setting the algorithm means that the checksums will be calculated during the file building process
//default behavior is to build without calculating checksums
void FileSystemBuilder::setAlgorithm(const std::string& algorithm) {
    this->algorithm = algorithm;
}

std::shared_ptr<File> IgnoreSymlinkFileSystemBuilder::build(const std::string& path) const {
    std::shared_ptr<ChecksumCalculator> calculator = nullptr;
    if (this->algorithm != "") {
        //user specified to calculate the checksums in the file building process
        calculator = ChecksumCalculatorFactory::getCalculator(this->algorithm);
    }
    return buildCommon(path, false, calculator);
}

//NOTE: this implementation will traverse UNIX symlinks only
//Windows .lnk files(or any other files) won't be treated as symlinks
std::shared_ptr<File> TraverseSymlinkFileSystemBuilder::build(const std::string& path) const {
    std::shared_ptr<ChecksumCalculator> calculator = nullptr;
    if (this->algorithm != "") {
        //user specified to calculate the checksums in the file building process
        calculator = ChecksumCalculatorFactory::getCalculator(this->algorithm);
    }
    return buildCommon(path, true, calculator);
}