#include "Course.h"
#include "Exceptions.h"
#include "Student.h"
#include "Teacher.h"
#include <iostream>

using namespace std;

// Constructor with basic validation
Course::Course(const string& n) : courseName(n) {
    if (n.empty()) {
        throw InvalidInputException("Course name cannot be empty.");
    }
    cout << "Successfully created " << n << " class." << endl;
}

const string& Course::getCourseName() const { 
    return courseName; 
}

// Enrolls a student and adds them to the observer list
void Course::enrollStudent(Student* s) {
    if (!s) {
        throw InvalidInputException("Cannot enroll a null student.");
    }

    // Check for duplicates
    for (auto* st : studentList) {
        if (st == s) {
            throw DuplicateEntityException(s->getName() + " is already enrolled in " + courseName);
        }
    }

    observers.push_back(s);
    studentList.push_back(s);
    cout << s->getName() << " added to " << courseName << endl;
}

// Assigns a teacher and adds them to the observer list
void Course::addTeacher(Teacher* t) {
    if (!t) {
        throw InvalidInputException("Cannot assign a null teacher.");
    }

    // Check for duplicates
    for (auto* tch : teacherList) {
        if (tch == t) {
            throw DuplicateEntityException(t->getName() + " is already teaching " + courseName);
        }
    }

    observers.push_back(t);
    teacherList.push_back(t);
    cout << "Teacher " << t->getName() << " joined " << courseName << " class." << endl;
}

// Displays all enrolled students
void Course::displayStudentList() const {
    if (studentList.empty()) { 
        cout << "(No students enrolled)\n"; 
        return; 
    }
    cout << "Students in " << courseName << ":\n";
    for (const auto* s : studentList) {
        if (s) cout << "- " << s->getName() << endl;
    }
}

// Displays assigned teachers
void Course::displayTeacherList() const {
    if (teacherList.empty()) { 
        cout << "(No teachers assigned)\n"; 
        return; 
    }
    cout << "Teachers for " << courseName << ":\n";
    for (const auto* t : teacherList) {
        if (t) cout << "- " << t->getName() << " (" << t->getSubject() << ")" << endl;
    }
}

// Notifies all observers (Students and Teachers) about a new post
void Course::post(IObserver* sender, const string& msg) {
    if (msg.empty()) {
        throw InvalidInputException("Course post message cannot be empty.");
    }

    cout << "\n--- New update in " << courseName << " ---" << endl;
    for (auto* o : observers) {
        if (o) {
            o->update(sender, msg);
        }
    }
    cout << "-----------------------------------" << endl;
}