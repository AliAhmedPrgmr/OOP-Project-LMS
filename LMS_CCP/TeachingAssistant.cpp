#include "TeachingAssistant.h"
#include <iostream>

using namespace std;

TeachingAssistant::TeachingAssistant(const string& n, int i, float c, const string& s)
    : User(n, i), Student(n, i, c), Teacher(n, i, s) {
    cout << "Success: " << n << " is registered as a teaching assistant!" << endl;
}

void TeachingAssistant::displayDashboard() const {
    cout << "\n==== Teaching Assistant Portal ====" << endl;
    cout << "Name: " << getName() << "\nID: " << getID() << "\nCGPA: " << getCGPA()
         << "\nEmail: " << getSubject() << endl;
    cout << "===================================" << endl;
}
