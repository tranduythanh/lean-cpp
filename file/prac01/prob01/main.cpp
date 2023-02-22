#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void typingAndSaveFile(const char* filename) {
    char data[100];

    // open a file in write mode.
    ofstream outfile;
    outfile.open(filename);

    cout << "Writing to the file" << endl;
    cout << "Type something: "; 
    cin.getline(data, 100);

    // write inputted data into the file.
    outfile << data << endl;
   
    // close the opened file.
    outfile.close();
}

void copyFile(const char* from, const char* dst) {
    ifstream infile(from);
    ofstream outfile(dst);

    // read input data from the file.
    string line;
    while (getline(infile, line, '\n')) {    
        outfile << line << endl;
    }

    // close the file.
    infile.close();
    outfile.close();
}

void appendFileContent(const char* org, const char* dst) {
    ifstream infile(dst);
    ofstream outfile;

    outfile.open(org, ios::app);

    string line;
    while (getline(infile, line, '\n')) {
        outfile << line << endl;
    }

    infile.close();
    outfile.close();
}

void mergeFiles(const char* file1, const char* file2, const char* dst) {
    ifstream infile1(file1);
    ifstream infile2(file2);
    ofstream outfile(dst);

    string line;
    while (getline(infile1, line, '\n')) {
        outfile << line << endl;
    }
    while (getline(infile2, line, '\n')) {
        outfile << line << endl;
    }
    infile1.close();
    infile2.close();
    outfile.close();
}

int main () {
    typingAndSaveFile("data.txt");
    copyFile("data.txt", "data2.txt");
    appendFileContent("data.txt", "data2.txt");
    mergeFiles("data.txt", "data2.txt", "data3.txt");
}