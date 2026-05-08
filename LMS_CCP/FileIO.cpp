#include "FileIO.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

void saveStudentsToFile(const vector<unique_ptr<Student>>& students, const string& filename) {
    ofstream ofs(filename, ios::trunc);
    for (const auto& s : students)
        ofs << s->getName() << "," << s->getID() << "," << s->getCGPA() << endl;
}

void loadStudentsFromFile(vector<unique_ptr<Student>>& students, const string& filename) {
    ifstream ifs(filename);
    string line;
    while (getline(ifs, line)) {
        stringstream ss(line);
        string name, id_str, cgpa_str;
        getline(ss, name, ',');
        getline(ss, id_str, ',');
        getline(ss, cgpa_str, ',');
        students.push_back(make_unique<Student>(name, stoi(id_str), stof(cgpa_str)));
    }
}

void saveTeachersToFile(const vector<unique_ptr<Teacher>>& teachers, const string& filename) {
    ofstream ofs(filename, ios::trunc);
    if (!ofs) { cerr << "Could not open file: " << filename << endl; return; }
    for (const auto& t : teachers)
        ofs << t->getName() << "," << t->getID() << "," << t->getSubject() << endl;
}

void loadTeachersFromFile(vector<unique_ptr<Teacher>>& teachers, const string& filename) {
    ifstream ifs(filename);
    if (!ifs) return;
    string line;
    while (getline(ifs, line)) {
        stringstream ss(line);
        string name, id_str, subject;
        getline(ss, name, ',');
        getline(ss, id_str, ',');
        getline(ss, subject);
        teachers.push_back(make_unique<Teacher>(name, stoi(id_str), subject));
    }
}

void saveCoursesToFile(const vector<unique_ptr<Course>>& courses, const string& filename) {
    ofstream ofs(filename, ios::trunc);
    if (!ofs) { cerr << "Could not open file: " << filename << endl; return; }
    for (const auto& c : courses) {
        ofs << c->getCourseName() << "|";
        for (size_t i = 0; i < c->teacherList.size(); ++i) {
            if (c->teacherList[i]) ofs << c->teacherList[i]->getID();
            if (i + 1 < c->teacherList.size()) ofs << ",";
        }
        ofs << "|";
        for (size_t i = 0; i < c->studentList.size(); ++i) {
            if (c->studentList[i]) ofs << c->studentList[i]->getID();
            if (i + 1 < c->studentList.size()) ofs << ",";
        }
        ofs << endl;
    }
}

void loadCoursesFromFile(vector<unique_ptr<Course>>& courses,
                         const vector<unique_ptr<Teacher>>& teachers,
                         const vector<unique_ptr<Student>>& students,
                         const string& filename) {
    ifstream ifs(filename);
    if (!ifs) return;
    string line;
    while (getline(ifs, line)) {
        stringstream ss(line);
        string courseName, teachersPart, studentsPart;
        getline(ss, courseName, '|');
        getline(ss, teachersPart, '|');
        getline(ss, studentsPart);
        auto course = make_unique<Course>(courseName);
        if (!teachersPart.empty()) {
            stringstream tss(teachersPart); string tidstr;
            while (getline(tss, tidstr, ',')) {
                int tid = stoi(tidstr);
                for (const auto& t : teachers)
                    if (t->getID() == tid) course->addTeacher(t.get());
            }
        }
        if (!studentsPart.empty()) {
            stringstream sss(studentsPart); string sidstr;
            while (getline(sss, sidstr, ',')) {
                int sid = stoi(sidstr);
                for (const auto& s : students)
                    if (s->getID() == sid) course->enrollStudent(s.get());
            }
        }
        courses.push_back(move(course));
    }
}