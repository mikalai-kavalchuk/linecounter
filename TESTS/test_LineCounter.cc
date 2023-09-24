#include <gtest/gtest.h>

#include "LinesCounter.h"

TEST(LineCounterTest, Count3LinesInFile) {
    
    std::string testFolderName = "test_dir/";
    std::string testFileName = testFolderName + "test_file1.txt";
    
    std::filesystem::create_directory(testFolderName);    
    std::ofstream file(testFileName);
    
    file << "Line 1\n Line 2\nLine 3";
    file.close();

    LinesCounter linesCounter = LinesCounter(testFolderName);
    std::size_t totalLineCount = linesCounter.processFiles();

    EXPECT_EQ(totalLineCount, 3);

    std::filesystem::remove(testFileName);
}

TEST(LineCounterTest, Count4LinesInFile) {

    std::string testFolderName = "test_dir/";
    std::string testFileName = testFolderName + "test_file2.txt";

    std::filesystem::create_directory(testFolderName);
    std::ofstream file(testFileName);

    // Must be counted as 4 lines because of 3 new line symbols 
    file << "Line 1\r\n Line 2\r\nLine 3\r\n";
    file.close();

    LinesCounter linesCounter = LinesCounter(testFolderName);
    std::size_t totalLineCount = linesCounter.processFiles();

    EXPECT_EQ(totalLineCount, 4);

    std::filesystem::remove(testFileName);
}

TEST(LineCounterTest, ExceprionInvalidDirectoryPath) {

    std::string testFolderName = "test_dir/";
    std::string testFileName = testFolderName + "its_not_a_folder.txt";

    std::filesystem::create_directory(testFolderName);
    std::ofstream file(testFileName); 
    file << "It's not a folder";
    file.close();

    try {
        LinesCounter linesCounter = LinesCounter(testFileName);
        linesCounter.processFiles();
    }
    catch (std::invalid_argument const& ex) {
        
        EXPECT_EQ(ex.what(), std::string("Invalid directory path."));        
    }

    std::filesystem::remove(testFileName);
    std::filesystem::remove(testFolderName);
}

TEST(LineCounterTest, ExceprionDirectoryDoesNotExist) {

    std::string testFolderName = "fake_dir";

    try {
        LinesCounter linesCounter = LinesCounter(testFolderName);
        linesCounter.processFiles();
    }
    catch (std::invalid_argument const& ex) {
        EXPECT_EQ(ex.what(), std::string("Directory does not exist."));
    }
}


