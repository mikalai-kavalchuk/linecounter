#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <mutex>
#include <thread>
#include <stdexcept>
#include <deque>


class LinesCounter {
public:
    LinesCounter() = delete;
    LinesCounter(std::string& directoryPath);
    std::size_t processFiles();

private:
    void counterThread(int threadId);
    std::filesystem::path getNextFile();
    std::size_t countLinesInFile(const std::filesystem::path& filePath);
    std::mutex fileNamesMutex;
    std::deque<std::filesystem::path> fileNames;
    std::atomic<std::size_t> totalLineCount = { 0 };
};