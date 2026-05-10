#include "FileIO.h"
#include "Exceptions.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// Saves all Student records to a CSV-style text file
void saveStudentsToFile(const vector<unique_ptr<Student>>& students, const string& filename) {
    ofstream ofs(filename, ios::trunc);
    if (!ofs) {
        throw InvalidOperationException("Failed to open " + filename + " for writing.");
    }
    for (const auto& s : students) {
        if (s) ofs << s->getName() << "," << s->getID() << "," << s->getCGPA() << endl;
    }
}

// Loads Student records and handles potential parsing errors
void loadStudentsFromFile(vector<unique_ptr<Student>>& students, const string& filename) {
    ifstream ifs(filename);
    if (!ifs) return; // Silent return if file doesn't exist yet

    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string name, id_str, cgpa_str;
        
        try {
            getline(ss, name, ',');
            getline(ss, id_str, ',');
            getline(ss, cgpa_str, ',');
            
            if (!name.empty() && !id_str.empty() && !cgpa_str.empty()) {
                students.push_back(make_unique<Student>(name, stoi(id_str), stof(cgpa_str)));
            }
        } catch (...) {
            // Skip corrupted lines
            continue;
        }
    }
}

// Saves Teacher records
void saveTeachersToFile(const vector<unique_ptr<Teacher>>& teachers, const string& filename) {
    ofstream ofs(filename, ios::trunc);
    if (!ofs) {
        cerr << "[File Error] Could not open file for writing: " << filename << endl;
        return;
    }
    for (const auto& t : teachers) {
        if (t) ofs << t->getName() << "," << t->getID() << "," << t->getSubject() << endl;
    }
}

// Loads Teacher records
void loadTeachersFromFile(vector<unique_ptr<Teacher>>& teachers, const string& filename) {
    ifstream ifs(filename);
    if (!ifs) return;

    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string name, id_str, subject;
        
        try {
            getline(ss, name, ',');
            getline(ss, id_str, ',');
            getline(ss, subject);
            
            if (!name.empty() && !id_str.empty()) {
                teachers.push_back(make_unique<Teacher>(name, stoi(id_str), subject));
            }
        } catch (...) {
            continue;
        }
    }
}

// Saves SystemAdmin records
void saveSystemAdminsToFile(const vector<unique_ptr<SystemAdmin>>& admins, const string& filename) {
    ofstream ofs(filename, ios::trunc);
    if (!ofs) {
        cerr << "[File Error] Could not open file for writing: " << filename << endl;
        return;
    }
    for (const auto& a : admins) {
        if (a) ofs << a->getName() << "," << a->getID() << endl;
    }
}

// Loads SystemAdmin records
void loadSystemAdminsFromFile(vector<unique_ptr<SystemAdmin>>& admins, const string& filename) {
    ifstream ifs(filename);
    if (!ifs) return;

    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string name, id_str;
        
        try {
            getline(ss, name, ',');
            getline(ss, id_str);
            
            if (!name.empty() && !id_str.empty()) {
                admins.push_back(make_unique<SystemAdmin>(name, stoi(id_str)));
            }
        } catch (...) {
            continue;
        }
    }
}

// Saves Course structure including Teacher and Student ID links
void saveCoursesToFile(const vector<unique_ptr<Course>>& courses, const string& filename) {
    ofstream ofs(filename, ios::trunc);
    if (!ofs) {
        cerr << "[File Error] Could not open file for writing: " << filename << endl;
        return;
    }
    for (const auto& c : courses) {
        if (!c) continue;
        
        ofs << c->getCourseName() << "|";
        
        // Save Teacher IDs linked to this course
        for (size_t i = 0; i < c->teacherList.size(); ++i) {
            if (c->teacherList[i]) ofs << c->teacherList[i]->getID();
            if (i + 1 < c->teacherList.size()) ofs << ",";
        }
        
        ofs << "|";
        
        // Save Student IDs linked to this course
        for (size_t i = 0; i < c->studentList.size(); ++i) {
            if (c->studentList[i]) ofs << c->studentList[i]->getID();
            if (i + 1 < c->studentList.size()) ofs << ",";
        }
        ofs << endl;
    }
}

// Reconstructs Course objects and re-links them to existing Teachers and Students
void loadCoursesFromFile(vector<unique_ptr<Course>>& courses,
                         const vector<unique_ptr<Teacher>>& teachers,
                         const vector<unique_ptr<Student>>& students,
                         const string& filename) {
    ifstream ifs(filename);
    if (!ifs) return;

    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string courseName, teachersPart, studentsPart;
        
        getline(ss, courseName, '|');
        getline(ss, teachersPart, '|');
        getline(ss, studentsPart);
        
        auto course = make_unique<Course>(courseName);
        
        // Re-link Teachers by ID
        if (!teachersPart.empty()) {
            stringstream tss(teachersPart); 
            string tidstr;
            while (getline(tss, tidstr, ',')) {
                try {
                    int tid = stoi(tidstr);
                    for (const auto& t : teachers) {
                        if (t->getID() == tid) course->addTeacher(t.get());
                    }
                } catch (...) {}
            }
        }
        
        // Re-link Students by ID
        if (!studentsPart.empty()) {
            stringstream sss(studentsPart); 
            string sidstr;
            while (getline(sss, sidstr, ',')) {
                try {
                    int sid = stoi(sidstr);
                    for (const auto& s : students) {
                        if (s->getID() == sid) course->enrollStudent(s.get());
                    }
                } catch (...) {}
            }
        }
        courses.push_back(move(course));
    }
}