#include "Teacher.h"
#include "Course.h"
#include <iostream>
using namespace std;

Teacher::Teacher(const string& n, int i, const string& s) : User(n, i), subject(s) {
    cout << "You are successfully registered as a teacher!" << endl;
}

void Teacher::displayDashboard() const {
    cout << "==== Teacher Portal ====" << endl;
    cout << "Teacher: " << getName() << " | Subject: " << subject << endl;
    cout << "Currently managing " << managedCourses.size() << " class(es):" << endl;
    for (const auto* course : managedCourses)
        if (course) cout << "* " << course->getCourseName() << endl;
    cout << "=========================" << endl;
}

void Teacher::announce(Course* c, const string& msg, IObserver* sender) {
    if (c) {
        cout << getName() << " announced in " << c->getCourseName() << endl;
        c->post(sender, msg);
    }
}

void Teacher::announce(Course* c, const string& msg) { announce(c, msg, this); }

void Teacher::addCourse(Course* c) {
    if (c) {
        for (auto* crs : managedCourses)
            if (crs == c) return;
        managedCourses.push_back(c);
        c->addTeacher(this);
    }
}

void Teacher::setSubject(const string& s) { subject = s; }
const string& Teacher::getSubject() const { return subject; }