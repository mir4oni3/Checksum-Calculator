#include "File.hpp"
#include "InputFacade.hpp"

class FileSystemBuilder {
public:
    virtual std::shared_ptr<File> build(const std::string&) = 0;
    virtual ~FileSystemBuilder() = default;
};

class IgnoreSymlinkFileSystemBuilder : public FileSystemBuilder {
public:
    std::shared_ptr<File> build(const std::string&) override;
};

class TraverseSymlinkFileSystemBuilder : public FileSystemBuilder {
public:
    std::shared_ptr<File> build(const std::string&) override;
};