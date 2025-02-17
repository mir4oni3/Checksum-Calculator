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
    static void compareChecksums(File& target, ChecksumCalculator& calc,
                                 const std::string& checksumFile, const std::string& format, ProgressReporter& reporter);
};