#include "Student.h"
#include "Course.h"
#include <iostream>
using namespace std;

Student::Student(const string& n, int i, float c) : User(n, i), cgpa(c) {
    cout << "You are successfully enrolled as a student!" << endl;
}

void Student::displayDashboard() const {
    cout << "==== Student Portal ====" << endl;
    cout << "Name: " << getName() << "\nID: " << getID() << "\nCGPA: " << cgpa << endl;
}

void Student::setCGPA(float n) { if (n >= 0.0 && n <= 4.0) cgpa = n; }
float Student::getCGPA() const { return cgpa; }

void Student::comment(Course* c, const string& msg, IObserver* sender) {
    if (c) {
        cout << getName() << " commented in " << c->getCourseName() << endl;
        c->post(sender, msg);
    }
}

void Student::comment(Course* c, const string& msg) { comment(c, msg, this); }