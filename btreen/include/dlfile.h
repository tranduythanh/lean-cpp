#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>

template <typename T>
void AppendObjToFile(const std::string& filename, const T& obj) {
    // Write the struct to the file
    std::ofstream outFile(filename, std::ios::binary | std::ios::app);
    
    // Write the struct to the file
    outFile.seekp(0, std::ios::end);
    
    // Write the struct to the file
    outFile.write(reinterpret_cast<const char*>(&obj), sizeof(T));

    // Close the file
    outFile.close();
}

template <typename T>
std::vector<T> readBinaryFile(const char* filename) {
    std::vector<T> objs;

    // Open the file in binary mode and seek to the end
    std::ifstream inFile(filename, std::ios::binary);
    inFile.seekg(0, std::ios::end);

    // Get the size of the file and calculate the number of structs
    std::streamsize fileSize = inFile.tellg();
    size_t numStructs = fileSize / sizeof(T);

    // Seek back to the beginning of the file and resize the vector
    inFile.seekg(0, std::ios::beg);
    objs.resize(numStructs);

    // Read the structs from the file
    inFile.read(reinterpret_cast<char*>(&objs[0]), fileSize);

    // Close the file
    inFile.close();

    return objs;
}

template <typename T>
using Condition = bool (*)(const T&);

template <typename T>
int fstreamSearchIndexOfObj(std::fstream& inFile, Condition<T> cond) {
    inFile.seekg(0, std::ios::beg);
    T obj;
    int i = 0;
    while(inFile.read(reinterpret_cast<char*>(&obj), sizeof(T))) {
        if(cond(obj)) {
            inFile.close();
            return i;
        }
        i++;
    }
    return -1;
}

template <typename T>
int searchIndexOfObj(const char* filename, Condition<T> cond) {
    std::fstream thisFile(filename, std::ios::binary | std::ios::in | std::ios::out);
    int index = fstreamSearchIndexOfObj(thisFile, cond);
    thisFile.close();
    return index;
}

template <typename T>
const T& fstreamReplaceObj(std::fstream& thisFile, int index, const T& obj) {
    T legacyT;
    size_t offset = sizeof(T)*index;
    
    thisFile.seekg(offset, std::ios::beg);
    thisFile.read(reinterpret_cast<char *>(&legacyT), sizeof(T));
    
    thisFile.seekp(offset, std::ios::beg);
    thisFile.write(reinterpret_cast<const char *>(&obj), sizeof(T));
    
    return legacyT;
}

template <typename T>
void fstreamAppendObj(std::fstream& thisFile, const T& obj) {
    thisFile.seekp(0, std::ios::end);
    thisFile.write(reinterpret_cast<const char*>(&obj), sizeof(T));
}

template <typename T>
void insertObj(const char* filename, int index, const T& obj) {
    std::fstream thisFile(filename, std::ios::in | std::ios::out | std::ios::binary);
    thisFile.seekg(0, std::ios::end);
    std::streamsize fileSize = thisFile.tellg();
    size_t numStructs = fileSize/sizeof(T);

    if (index >= numStructs) {
        fstreamAppendObj(thisFile, obj);
        thisFile.close();
        return;
    }

    T tmpObj = obj;
    for (int i=index; i<numStructs; i++) {
        tmpObj = fstreamReplaceObj(thisFile, i, tmpObj);
    }

    fstreamAppendObj(thisFile, tmpObj);
    thisFile.close();
}

template <typename T>
size_t fstreamDeleteObj(std::fstream& thisFile, int index) {
    thisFile.seekg(0, std::ios::end);
    std::streamsize fileSize = thisFile.tellg();
    size_t numStructs = fileSize / sizeof(T);

    // read the very last obj
    T obj;
    thisFile.seekg(-sizeof(T), std::ios::end);
    thisFile.read(reinterpret_cast<char *>(&obj), sizeof(T));

    for (int i=numStructs-2; i>=index; i--) {
        obj = fstreamReplaceObj(thisFile, i, obj);
    }

    // return new end of file
    return (numStructs-1)*sizeof(T);
}

template <typename T>
void deleteObj(const char* filename, int index) {
    std::fstream thisFile(filename, std::ios::in | std::ios::out | std::ios::binary);
    size_t newEnd = fstreamDeleteObj<T>(thisFile, index);

    // open new temporarily file
    const char* tmpFilename = "tmp.dat";
    std::fstream thatFile(tmpFilename, std::ios::out | std::ios::binary);

    // copy content from thisFile to thatFile
    char buffer[newEnd];
    thisFile.seekg(0, std::ios::beg);
    thisFile.read(buffer, newEnd);
    thatFile.write(buffer, newEnd);

    thisFile.close();
    thatFile.close();

    std::cout << "ahihi" << std::endl;
    rename(tmpFilename, filename);
}

template <typename T>
void replaceObj(const char* filename, int index, const T& obj) {
    std::fstream thisFile(filename, std::ios::in | std::ios::out | std::ios::binary);
    fstreamReplaceObj<T>(thisFile, index, obj);
    thisFile.close();
}

template <typename T>
std::vector<T> readBinaryFileWithCondtion(const char* filename, Condition<T> cond) {
    std::vector<T> objs;
    
    std::ifstream inFile(filename, std::ios::binary);
    
    inFile.seekg(0, std::ios::end);
    std::streamsize fileSize = inFile.tellg();

    size_t numStructs = fileSize / sizeof(T);
    
    inFile.seekg(0, std::ios::beg);

    for (size_t i = 0; i < numStructs; ++i) {
        T obj;
        inFile.read(reinterpret_cast<char*>(&obj), sizeof(T));
        if (cond(obj)) {
            objs.push_back(obj);
        }
    }
    
    return objs;
}
