#include "Files/File.hpp"
#include "Visitors/VisitorWriter.hpp"

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

std::string RegularFile::getChecksum(const ChecksumCalculator& calc) const {
    if (this->checksum == "") {
        std::ifstream file(this->getPath(), std::ios::binary);
        if (!file) {
            throw std::runtime_error("RegularFile::getChecksum - Error opening file");
        }
        this->checksum = calc.calculate(file);
    }
    return this->checksum;
}

void RegularFile::accept(const VisitorWriter& writer) const {
    writer.visitRegularFile(*this);
}

Directory::Directory(const std::string& path) : File(path) {
    if (!std::filesystem::is_directory(path)) {
        throw std::invalid_argument("Directory::Directory - Not a directory");
    }
}

void Directory::addFile(std::unique_ptr<File>&& file) {
    if (!file) {
        throw std::invalid_argument("Directory::addFile - Null file");
    }
    this->files.push_back(std::move(file));
}

const std::vector<std::unique_ptr<File>>& Directory::getFiles() const {
    return this->files;
}

void Directory::sortFiles() {
    std::sort(this->files.begin(), this->files.end(), [](const std::unique_ptr<File>& a, const std::unique_ptr<File>& b) {
        return a->getPath() < b->getPath();
    });
}
void Directory::accept(const VisitorWriter& writer) const {
    writer.visitDirectory(*this);
}