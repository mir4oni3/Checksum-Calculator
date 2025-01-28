#include "IO/UserActions.hpp"
#include "Factories/CalculatorFactory.hpp"
#include "Factories/HashStreamWriterFactory.hpp"
#include "Factories/FileSystemBuilderFactory.hpp"
#include "Files/FileIterator.hpp"
#include "Observers/ProgressReporter.hpp"

#include <memory>

void UserActions::start(const InputFacade& input) {
    //initialize builder
    std::shared_ptr<FileSystemBuilder> builder = FileSystemBuilderFactory::getBuilder(input.getTraverse());

    //set builder mode to calculate checksums when building filesystem if requested
    if (input.getBuildChecksums()) {
        builder->setAlgorithm(input.getAlgorithm());
    }

    //initialize other objects
    std::shared_ptr<File> target = builder->build(input.getPath());
    std::shared_ptr<ChecksumCalculator> calc = ChecksumCalculatorFactory::getCalculator(input.getAlgorithm());
    std::shared_ptr<ProgressReporter> reporter = std::make_shared<ProgressReporter>(std::cout);
    std::shared_ptr<HashStreamWriter> writer = HashStreamWriterFactory::getWriter(input.getFormat(), std::cout, calc);

    //add reporter as observer to the HashStreamWriter
    writer->addObserver(reporter);

    //perform actions
    if (input.getChecksums() == "") {
        viewChecksums(target, writer);
    }
    else {
        compareChecksums(target, calc, input.getChecksums(), writer);
    }

    //save file if user requested
    if (input.getSaveTo() != "") {
        std::ofstream ofs(input.getSaveTo());
        if (!ofs) {
            throw std::runtime_error("UserActions::start - Error opening file");
        }
        HashStreamWriterFactory::getWriter(input.getFormat(), ofs, calc)->exportFile(target);
    }
}

void UserActions::viewChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<HashStreamWriter>& writer) {
    if (!writer) {	
        throw std::invalid_argument("UserActions::viewChecksums - nullptr parser passed");
    }
    writer->exportFile(target);
}

void UserActions::compareChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<ChecksumCalculator>& calc, const std::string& checksumFile, const std::shared_ptr<HashStreamWriter>& writer) {
    if (!target || !calc || !writer) {
        throw std::invalid_argument("UserActions::compareChecksums - nullptr arg(s) passed");
    }

    //parse checksum file to hashmap
    std::ifstream checksumIfs(checksumFile);
    if (!checksumIfs) {
        throw std::runtime_error("FileActions::viewChecksums - Error opening file");
    }
    std::unordered_map<std::string, std::string> checksums = writer->parseFiles(checksumIfs);
    
    FileIterator it(target);

    //iterate over target files and compare with the hashmap
    while (auto current = it.next()) {
        std::shared_ptr<RegularFile> regFile = std::dynamic_pointer_cast<RegularFile>(current);
        if (!regFile) {
            continue;
        }

        std::string currentPath = regFile->getPath();
        std::string checksum = regFile->getChecksum(calc);

        if (checksums.find(currentPath) == checksums.end()) {
            std::cout << currentPath << ": NEW\n";
            continue;
        }
        if (checksum != checksums.at(currentPath)) {
            std::cout << currentPath << ": MODIFIED\n";
            checksums.erase(currentPath);
            continue;
        }
        if (checksum == checksums.at(currentPath)) {
            std::cout << currentPath << ": OK\n";
            checksums.erase(currentPath);
            continue;
        }
    }

    for (const auto& [path, checksum] : checksums) {
        std::cout << path << ": REMOVED\n";
    }
}