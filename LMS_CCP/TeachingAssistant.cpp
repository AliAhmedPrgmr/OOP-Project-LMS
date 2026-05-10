#include "TeachingAssistant.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;

namespace {
constexpr const char* DEFAULT_TA_SUBJECT = "Teaching Assistant";
}

TeachingAssistant::TeachingAssistant(const string& n, int i, float c, const string& e)
    : User(n, i), Student(n, i, c), Teacher(n, i, DEFAULT_TA_SUBJECT), email(e) {
    if (email.empty()) {
        throw InvalidInputException("Teaching assistant email cannot be empty.");
    }
    cout << "Success: " << n << " is registered as a teaching assistant!" << endl;
}

const string& TeachingAssistant::getEmail() const {
    return email;
}

void TeachingAssistant::displayDashboard() const {
    cout << "\n==== Teaching Assistant Portal ====" << endl;
    cout << "Name: " << getName() << "\nID: " << getID() << "\nCGPA: " << getCGPA()
         << "\nEmail: " << getEmail() << endl;
    cout << "===================================" << endl;
}
