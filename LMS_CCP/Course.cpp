#include "Course.h"
#include "Student.h"
#include "Teacher.h"
#include <iostream>
using namespace std;

Course::Course(const string& n) : courseName(n) {
    cout << "Successfully created " << n << " class." << endl;
}

const string& Course::getCourseName() const { return courseName; }

void Course::enrollStudent(Student* s) {
    if (s) {
        for (auto* st : studentList)
            if (st == s) { cout << s->getName() << " is already enrolled in " << courseName << endl; return; }
        observers.push_back(s);
        studentList.push_back(s);
        cout << s->getName() << " added to " << courseName << endl;
    }
}

void Course::addTeacher(Teacher* t) {
    if (t) {
        for (auto* tch : teacherList)
            if (tch == t) { cout << t->getName() << " is already teaching " << courseName << endl; return; }
        observers.push_back(t);
        teacherList.push_back(t);
        cout << "Teacher " << t->getName() << " joined " << courseName << " class" << endl;
    }
}

void Course::displayStudentList() const {
    if (studentList.empty()) { cout << "(No students enrolled)\n"; return; }
    for (const auto* s : studentList) if (s) cout << "- " << s->getName() << endl;
}

void Course::displayTeacherList() const {
    if (teacherList.empty()) { cout << "(No teachers assigned)\n"; return; }
    for (const auto* t : teacherList) if (t) cout << "- " << t->getName() << " (" << t->getSubject() << ")" << endl;
}

void Course::post(IObserver* sender, const string& msg) {
    cout << "New update in " << courseName << ":\n";
    for (auto* o : observers) if (o) o->update(sender, msg);
    cout << endl;
}