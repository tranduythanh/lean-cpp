#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>

#include "dlfile.h"

using namespace std;

struct Student
{
    int         ID;
    std::string Name;
    int    DOB;
    float  Math;
    float  Physics;
    float  Chemistry;
    char   Grade;
    int    Deleted;
};

void debugStudent(const Student& obj) {
    std::cout   << obj.ID           << "\t"
                << obj.Name         << "\t"
                << obj.DOB          << "\t"
                << obj.Math         << "\t"
                << obj.Physics      << "\t"
                << obj.Chemistry    << "\t"
                << obj.Grade        << "\t"
                << obj.Deleted
                << std::endl;
}

bool cond1(const Student& obj) {
    return obj.DOB == 2000;
}

bool cond2(const Student& obj) {
    return obj.DOB == 2001;
}

void printFile(const char* filename) {
    std::cout << "Read and print all" << std::endl;
    std::vector<Student> objs = readBinaryFile<Student>(filename);
    for (int i=0; i < objs.size(); i++) {
        debugStudent(objs[i]);
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

    AppendObjToFile<Student>("data.dat", s1);
    AppendObjToFile<Student>("data.dat", s2);
    AppendObjToFile<Student>("data.dat", s3);
    AppendObjToFile<Student>("data.dat", s4);

    printFile("data.dat");

    cout << "Read and print all students those are born in 2000" << endl;
    vector<Student> students2 = readBinaryFileWithCondtion<Student>("data.dat", cond1);
    for (int i=0; i < students2.size(); i++) {
        debugStudent(students2[i]);
    }

    cout << "Search for students that are born in 2000" << endl;
    int index = searchIndexOfObj<Student>("data.dat", cond1);
    cout << index << endl;

    cout << "insert new student right after first student born in 2000" << endl;
    index = searchIndexOfObj<Student>("data.dat", cond1);
    if (index < 0) {
        cout << "no student satisfies your condition" << endl;
        return 0;
    }
    Student s5 = {555, "Tường",   1995, 9.0, 9.1, 9.2, 'D', 1};
    insertObj("data.dat", index, s5);
    printFile("data.dat");

    cout << "replace first student born in 2000 by a new student" << endl;
    index = searchIndexOfObj<Student>("data.dat", cond1);
    if (index < 0) {
        cout << "no student satisfies your condition" << endl;
        return 0;
    }
    Student s6 = {777, "Mây", 2007, 9.0, 9.1, 9.2, 'D', 1};
    replaceObj("data.dat", index, s6);
    printFile("data.dat");

    cout << "delete first student born in 2001" << endl;
    index = searchIndexOfObj<Student>("data.dat", cond2);
    if (index < 0) {
        cout << "no student satisfies your condition" << endl;
        return 0;
    }
    softDeleteObj<Student>("data.dat", index);
    printFile("data.dat");
}
