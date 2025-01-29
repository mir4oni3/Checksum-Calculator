#include "IO/UserActions.hpp"
#include "Factories/CalculatorFactory.hpp"
#include "Factories/HashStreamWriterFactory.hpp"
#include "Factories/FileSystemBuilderFactory.hpp"
#include "Factories/ParserFactory.hpp"
#include "Files/FileIterator.hpp"
#include "Visitors/CompareWriter.hpp"

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
    std::shared_ptr<FileHashParser> parser = ParserFactory::getParser(input.getFormat());

    //perform actions
    if (input.getChecksums() == "") { 
        writer->addObserver(reporter);
        viewChecksums(target, writer);
    }
    else {
        //parse checksum file to hashmap
        std::ifstream checksumIfs(input.getChecksums());
        if (!checksumIfs) {
            throw std::runtime_error("FileActions::start - Error opening checksum file");
        }
        std::unordered_map<std::string, std::string> checksums = parser->parseFiles(checksumIfs);

        compareChecksums(target, calc, checksums, reporter);
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

void UserActions::compareChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<ChecksumCalculator>& calc,
                                   std::unordered_map<std::string, std::string>& checksums, const std::shared_ptr<ProgressReporter>& reporter) {
    if (!target || !calc || !reporter) {
        throw std::invalid_argument("UserActions::compareChecksums - nullptr arg(s) passed");
    }
    std::shared_ptr<CompareWriter> compareWriter = std::make_shared<CompareWriter>(std::cout, calc, checksums);
    compareWriter->addObserver(reporter);
    compareWriter->exportFile(target);
}