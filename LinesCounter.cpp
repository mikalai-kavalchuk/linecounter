#include "LinesCounter.h"

LinesCounter::LinesCounter(std::string& directoryPath) {

    if (!std::filesystem::exists(directoryPath)) {
        throw std::invalid_argument("Directory does not exist.");
    }

    if (!std::filesystem::is_directory(directoryPath)) {
        throw std::invalid_argument("Invalid directory path.");
    }

    // Get file names in a given directory
    for (const auto& file : std::filesystem::directory_iterator(directoryPath)) {
        if (file.is_regular_file()) {
            fileNames.emplace_back(file.path());
        }
    }
}


std::size_t LinesCounter::countLinesInFile(const std::filesystem::path& filePath) {
    std::size_t lineCount = 0;
    std::ifstream stream(filePath);
    if (stream) {
        stream.seekg(0);
        lineCount = std::count(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>(), '\n');
        // If the file stream not ends with EOL, then lineCount should be incremented
        if (stream.tellg() != 0) {
            stream.unget();
            if (stream.get() != '\n') { ++lineCount; }
        }
    }
    return lineCount;
}



std::filesystem::path LinesCounter::getNextFile() {

    // Use a lexical scope and lock_guard to safely lock the mutex
    std::lock_guard<std::mutex> lock(fileNamesMutex);

    if (fileNames.empty()) {
        return {};
    }
    else {
        std::filesystem::path nextPath = fileNames.front();
        fileNames.pop_front();
        return nextPath;
    }    
}


void LinesCounter::counterThread(int threadId) {   

    std::size_t lineCount = 0;
    std::filesystem::path file = getNextFile();

    for (; file.has_filename(); file = getNextFile()) {
        
        lineCount += countLinesInFile(file);
    }

    // Update the total line count in the end of thread
    totalLineCount.fetch_add(lineCount, std::memory_order_relaxed);
}


std::size_t LinesCounter::processFiles() {

    std::vector<std::thread> threads;

    if (fileNames.size() == 0) return 0;

    // Get avaliable number of CPUs
    const auto cpuNum = std::thread::hardware_concurrency();    

    // Start needed number of threads
    for (unsigned int threadId = 0; threadId < cpuNum; ++threadId) {
        threads.emplace_back(&LinesCounter::counterThread, this, threadId);
    }

    // Wait for threads to end their job
    for (auto& t : threads) {
        t.join();
    }

    return totalLineCount;
}