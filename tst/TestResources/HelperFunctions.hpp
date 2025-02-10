#pragma once

#include <filesystem>
#include <string>
#include <memory>

#include "Files/File.hpp"

namespace HelperTestFunctions {
    inline std::filesystem::path getResourceFile(const std::string& filename) {
        std::filesystem::path sourcePath = __FILE__; 
        std::filesystem::path sourceDir = sourcePath.parent_path().parent_path();  

        std::filesystem::path path = sourceDir / "TestResources";
        path = path / filename;
        path = std::filesystem::absolute(path);
        return path;
    }

    inline std::string getResourceFilePath(const std::string& filename) {
        return getResourceFile(filename).string();
    }
}