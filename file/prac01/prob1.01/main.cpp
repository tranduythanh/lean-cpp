#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Student
{
    int    ID;
    string Name;
    int    DOB;
    float  Math;
    float  Physics;
    float  Chemistry;
    char   Grade;
    int    Deleted;
};

void debugStudent(const Student& student) {
    cout    << student.ID           << "\t"
            << student.Name         << "\t"
            << student.DOB          << "\t"
            << student.Math         << "\t"
            << student.Physics      << "\t"
            << student.Chemistry    << "\t"
            << student.Grade        << "\t"
            << student.Deleted
            << endl;
}

bool cond1(const Student& student) {
    return student.DOB == 2000;
}

bool cond2(const Student& student) {
    return student.DOB == 2001;
}

void AppendStudentToFile(const string& filename, const Student& student) {
    // Write the struct to the file
    ofstream outFile(filename, ios::binary | ios::app);
    
    // Write the struct to the file
    outFile.seekp(0, std::ios::end);
    
    // Write the struct to the file
    outFile.write(reinterpret_cast<const char*>(&student), sizeof(Student));

    // Close the file
    outFile.close();
}

vector<Student> readBinaryFile(const char* filename) {
    vector<Student> students;

    // Open the file in binary mode and seek to the end
    std::ifstream inFile(filename, std::ios::binary);
    inFile.seekg(0, std::ios::end);

    // Get the size of the file and calculate the number of structs
    streamsize fileSize = inFile.tellg();
    size_t numStructs = fileSize / sizeof(Student);

    // Seek back to the beginning of the file and resize the vector
    inFile.seekg(0, std::ios::beg);
    students.resize(numStructs);

    // Read the structs from the file
    inFile.read(reinterpret_cast<char*>(&students[0]), fileSize);

    // Close the file
    inFile.close();

    return students;
}

using Condition = bool (*)(const Student&);

int fstreamSearchIndexOfStudent(fstream& inFile, Condition cond) {
    inFile.seekg(0, ios::beg);
    Student student;
    int i = 0;
    while(inFile.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        if(cond(student)) {
            inFile.close();
            return i;
        }
        i++;
    }
    return -1;
}

int searchIndexOfStudent(const char* filename, Condition cond) {
    fstream thisFile(filename, ios::binary | ios::in | ios::out);
    int index = fstreamSearchIndexOfStudent(thisFile, cond);
    thisFile.close();
    return index;
}


const Student& fstreamReplaceStudent(fstream& thisFile, int index, const Student& student) {
    Student legacyStudent;
    size_t offset = sizeof(Student)*index;
    
    thisFile.seekg(offset, ios::beg);
    thisFile.read(reinterpret_cast<char *>(&legacyStudent), sizeof(Student));
    
    thisFile.seekp(offset, ios::beg);
    thisFile.write(reinterpret_cast<const char *>(&student), sizeof(Student));
    
    return legacyStudent;
}

void fstreamAppendStudent(fstream& thisFile, const Student& student) {
    thisFile.seekp(0, ios::end);
    thisFile.write(reinterpret_cast<const char*>(&student), sizeof(Student));
}

void insertStudent(const char* filename, int index, const Student& student) {
    fstream thisFile(filename, ios::in | ios::out | ios::binary);
    thisFile.seekg(0, ios::end);
    streamsize fileSize = thisFile.tellg();
    size_t numStructs = fileSize/sizeof(Student);

    if (index >= numStructs) {
        fstreamAppendStudent(thisFile, student);
        thisFile.close();
        return;
    }

    Student tmpStudent = student;
    for (int i=index; i<numStructs; i++) {
        tmpStudent = fstreamReplaceStudent(thisFile, i, tmpStudent);
    }

    fstreamAppendStudent(thisFile, tmpStudent);
    thisFile.close();
}

void fstreamDeleteStudent(fstream& thisFile, int index) {
    thisFile.seekg(0, ios::end);
    streamsize fileSize = thisFile.tellg();
    size_t numStructs = fileSize / sizeof(Student);

    cout << numStructs << "\t" << fileSize << "\t" << sizeof(Student) << endl;

    // read the very last student
    Student student;
    thisFile.seekg(-sizeof(Student), ios::end);
    thisFile.read(reinterpret_cast<char *>(&student), sizeof(Student));

    cout << "==========" << endl;

    for (int i=numStructs-2; i>=index; i--) {
        student = fstreamReplaceStudent(thisFile, i, student);
    }

    cout << "==========" << endl;

    thisFile.seekp(-sizeof(Student), ios::end);
    cout << "==========" << endl;
    thisFile.write("", sizeof(Student));
    cout << "==========" << endl;
}

void deleteStudent(const char* filename, int index) {
    fstream thisFile(filename, ios::in | ios::out | ios::binary);
    fstreamDeleteStudent(thisFile, index);
    thisFile.close();
}

void replaceStudent(const char* filename, int index, const Student& student) {
    fstream thisFile(filename, ios::in | ios::out | ios::binary);
    fstreamReplaceStudent(thisFile, index, student);
    thisFile.close();
}

vector<Student> readBinaryFileWithCondtion(const char* filename, Condition cond) {
    vector<Student> students;
    
    ifstream inFile(filename, ios::binary);
    
    inFile.seekg(0, ios::end);
    streamsize fileSize = inFile.tellg();

    size_t numStructs = fileSize / sizeof(Student);
    
    inFile.seekg(0, ios::beg);

    for (size_t i = 0; i < numStructs; ++i) {
        Student student;
        inFile.read(reinterpret_cast<char*>(&student), sizeof(Student));
        if (cond(student)) {
            students.push_back(student);
        }
    }
    
    return students;
}

void printFile(const char* filename) {
    cout << "Read and print all" << endl;
    vector<Student> students = readBinaryFile(filename);
    for (int i=0; i < students.size(); i++) {
        debugStudent(students[i]);
    }
}

int main () {
    cout << "Just writing nothing to clear file content" << endl;
    ofstream outFile("data.dat", ios::binary | ios::out);
    Student s0 = {};
    outFile.write(reinterpret_cast<const char*>(&s0), 0);
    outFile.close();

    cout << "Append structs to file" << endl;
    Student s1 = { 1, "Thanh", 1991, 9.0, 9.1, 9.2, 'A', 1};
    Student s2 = { 2, "Tín",   2001, 9.0, 9.1, 9.2, 'B', 1};
    Student s3 = { 3, "Đạt",   2000, 9.0, 9.1, 9.2, 'C', 1};
    Student s4 = { 4, "Vĩnh",  2007, 9.0, 9.1, 9.2, 'C', 1};

    AppendStudentToFile("data.dat", s1);
    AppendStudentToFile("data.dat", s2);
    AppendStudentToFile("data.dat", s3);
    AppendStudentToFile("data.dat", s4);

    printFile("data.dat");

    cout << "Read and print all students those are born in 2000" << endl;
    vector<Student> students2 = readBinaryFileWithCondtion("data.dat", cond1);
    for (int i=0; i < students2.size(); i++) {
        debugStudent(students2[i]);
    }

    cout << "Search for students that are born in 2000" << endl;
    int index = searchIndexOfStudent("data.dat", cond1);
    cout << index << endl;

    cout << "insert new student right after first student born in 2000" << endl;
    index = searchIndexOfStudent("data.dat", cond1);
    if (index < 0) {
        cout << "no student satisfies your condition" << endl;
        return 0;
    }
    Student s5 = {555, "Tường",   1995, 9.0, 9.1, 9.2, 'D', 1};
    insertStudent("data.dat", index, s5);
    printFile("data.dat");

    cout << "replace first student born in 2000 by a new student" << endl;
    index = searchIndexOfStudent("data.dat", cond1);
    if (index < 0) {
        cout << "no student satisfies your condition" << endl;
        return 0;
    }
    Student s6 = {777, "Mây", 2007, 9.0, 9.1, 9.2, 'D', 1};
    replaceStudent("data.dat", index, s6);
    printFile("data.dat");


    cout << "delete first student born in 2001" << endl;
    index = searchIndexOfStudent("data.dat", cond2);
    if (index < 0) {
        cout << "no student satisfies your condition" << endl;
        return 0;
    }
    deleteStudent("data.dat", index);
    printFile("data.dat");
}
