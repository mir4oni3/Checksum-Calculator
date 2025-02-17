#pragma once

#include "File.hpp"
#include "IO/InputFacade.hpp"
#include "Calculators/ChecksumCalculator.hpp"

#include <unordered_set>

class FileSystemBuilder {
protected:
    std::unique_ptr<ChecksumCalculator> calc = nullptr;
    bool saveFullPath = false;
    bool sortFiles = false;

public:
    virtual ~FileSystemBuilder() = default;

    std::unique_ptr<File> build(const std::string&) const;
    
    void setAlgorithm(const std::string& algorithm);
    void setSortFiles(bool sort);
    void saveFullPaths(bool save);

protected:
    virtual std::unique_ptr<File> buildCommon(const std::string& path) const;
    std::unique_ptr<File> handleDirectory(const std::string& path) const;
};

//NOTE: this implementation will traverse UNIX symlinks only
//Windows .lnk files(or any other files) won't be treated as symlinks
class TraverseSymlinkFileSystemBuilder : public FileSystemBuilder {
    mutable std::unordered_set<std::string> visited;
protected:
    std::unique_ptr<File> buildCommon(const std::string& path) const override;
};