#include <iostream>
#include <stdexcept>

#include "LinesCounter.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: linecounter <path-to-a-directory>";
        return 1;
    }
 
    std::string directoryPath = argv[1];
    
    try {
        LinesCounter linesCounter = LinesCounter(directoryPath);
        std::size_t totalLineCount = linesCounter.processFiles();
        std::cout << totalLineCount << std::endl;
        return 0;
    }
    catch (std::invalid_argument const& ex){
        std::cout << "Error: " << ex.what() << '\n';
        return 1;
    }
}

