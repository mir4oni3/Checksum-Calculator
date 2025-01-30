#pragma once

#include <string>

enum class ObserverMessage {
    newRegularFile,
    newDirectory,
    rootFileSize,
    progress,
    allFilesHandled,
    pauseCalculation,
    resumeCalculation
};