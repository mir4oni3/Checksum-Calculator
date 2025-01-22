#pragma once

#include "File.hpp"
#include "InputFacade.hpp"

class FileSystemBuilder {
protected:
    const InputFacade& input;
public:
    FileSystemBuilder(const InputFacade& input);
    virtual ~FileSystemBuilder() = default;

    virtual std::shared_ptr<File> build(const std::string&) const = 0;
};

class IgnoreSymlinkFileSystemBuilder : public FileSystemBuilder {
public:
    IgnoreSymlinkFileSystemBuilder(const InputFacade& input);

    std::shared_ptr<File> build(const std::string&) const override;
};

class TraverseSymlinkFileSystemBuilder : public FileSystemBuilder {
public:
    TraverseSymlinkFileSystemBuilder(const InputFacade& input);
    
    std::shared_ptr<File> build(const std::string&) const override;
};