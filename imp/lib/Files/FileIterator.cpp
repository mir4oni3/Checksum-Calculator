#include "Files/FileIterator.hpp"

#include <iostream>

FileIterator::FileIterator(const File& root) {
    stack.push({root, 0});
}

bool FileIterator::hasNext() const {
    return !stack.empty();
}

const File& FileIterator::next() {
    //DFS file traversal implementation with a stack
    if (stack.empty()) {
        throw std::invalid_argument("FileIterator::next - No more files to return");
    }

    auto [file, index] = stack.top();
    stack.pop();

    try { //executed if file is a directory
        const Directory& dir = dynamic_cast<const Directory&>(file);
        const auto& dirfiles = dir.getFiles();

        if (index < dirfiles.size()) {
            if (index < dirfiles.size() - 1) {
                //push directory again with incremented index
                //if there are more subfiles in it
                stack.push({dir, index + 1});
            }
            //push the next file
            stack.push({*dirfiles[index], 0});
        }

    } catch (const std::bad_cast& e) {
        //not a directory, do nothing
    }

    if (index == 0) {
        //either a regular file or an yet unreturned directory
        return file;
    }

    //file is an already returned directory, skip it
    return next();
}