#pragma once

#include "../inc/InputFacade.hpp"
#include "../inc/ChecksumCalculator.hpp"
#include "../inc/File.hpp"
#include "../inc/FileParser.hpp"

#include <memory>

class UserActions {
public:
    static void start(const InputFacade& input);
    static void viewChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<ChecksumCalculator>& calc, const std::shared_ptr<FileParser>& parser);
    static void compareChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<ChecksumCalculator>& calc, std::unordered_map<std::string, std::string>& checksums);
};