#include "../inc/FileIterator.hpp"

FileIterator::FileIterator(const std::shared_ptr<File>& root) {
    if (!root) {
        throw std::invalid_argument("FileIterator::FileIterator - nullptr arg");
    }
    stack.push({root, 0});
}

bool FileIterator::hasMore() {
    return !stack.empty();
}

std::shared_ptr<File> FileIterator::next() {
    //DFS file traversal implementation with a stack
    if (!hasMore()) {
        throw std::out_of_range("AllFileIterator::next - no more files");
    }

    auto [file, index] = stack.top();
    stack.pop();

    if (auto dir = std::dynamic_pointer_cast<Directory>(file)) {
        auto dirfiles = dir->getFiles();
        if (index < dirfiles.size()) {
            //push directory again with incremented index
            stack.push({dir, index + 1});
            //push only the next file
            stack.push({dirfiles[index], 0});
        }
    }

    if (index == 0) {
        //either a regular file or an yet unreturned directory
        return file;
    }

    //file is an already returned directory, skip it
    return next();
}