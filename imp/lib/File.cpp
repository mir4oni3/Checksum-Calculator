#include "../inc/File.hpp"

#include <filesystem>

File::File(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        throw std::invalid_argument("File::File - File does not exist");
    }
    this->path = path;
}

void File::setSize(size_t size) {
    this->size = size;
    this->sizeKnown = true;
}

size_t File::getSize() const {
    if (!this->sizeKnown) {
        throw std::logic_error("File::getSize - Size is not known");
    }
    return this->size;
}

std::string File::getPath() const {
    return this->path;
}

RegularFile::RegularFile(const std::string& path) : File(path) {
    if (!std::filesystem::is_regular_file(path)) {
        throw std::invalid_argument("RegularFile::RegularFile - Not a regular file");
    }
}

Directory::Directory(const std::string& path) : File(path) {
    if (!std::filesystem::is_directory(path)) {
        throw std::invalid_argument("Directory::Directory - Not a directory");
    }
}

void Directory::addFile(std::shared_ptr<File>& file) {
    if (!file) {
        throw std::invalid_argument("Directory::addFile - Invalid file");
    }
    this->files.push_back(file);
}

