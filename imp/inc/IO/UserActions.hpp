#pragma once

#include "InputFacade.hpp"
#include "Calculators/ChecksumCalculator.hpp"
#include "Files/File.hpp"
#include "Visitors/HashStreamWriter.hpp"
#include "Observers/ProgressReporter.hpp"

#include <memory>
#include <unordered_map>

class UserActions {
public:
    static void start(const InputFacade& input);
    static void viewChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<HashStreamWriter>& writer);
    static void compareChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<ChecksumCalculator>& calc,
                                 const std::string& checksumFile, const std::string& format, const std::shared_ptr<ProgressReporter>& reporter);
};