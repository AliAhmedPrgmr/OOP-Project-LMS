#include "Student.h"
#include "Course.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;

// Constructor: Validates CGPA range and initializes student
Student::Student(const string& n, int i, float c) : User(n, i), cgpa(c) {
    if (c < 0.0f || c > 4.0f) {
        throw InvalidInputException("Student CGPA must be between 0.0 and 4.0.");
    }
    cout << "Success: " << n << " is enrolled as a student!" << endl;
}

// Displays personal student info in the portal
void Student::displayDashboard() const {
    cout << "\n==== Student Portal ====" << endl;
    cout << "Name: " << getName() << "\nID: " << getID() << "\nCGPA: " << cgpa << endl;
    cout << "========================" << endl;
}

// Safely updates the student's CGPA
void Student::setCGPA(float n) {
    if (n < 0.0f || n > 4.0f) {
        throw InvalidInputException("CGPA must be between 0.0 and 4.0.");
    }
    cgpa = n;
}

float Student::getCGPA() const { 
    return cgpa; 
}

// Overloaded comment method: sends a message to the course observers
void Student::comment(Course* c, const string& msg, IObserver* sender) {
    if (!c) {
        throw InvalidInputException("Action failed: Target course cannot be null.");
    }
    if (msg.empty()) {
        throw InvalidInputException("Action failed: Comment message cannot be empty.");
    }
    
    cout << "[Student Log] " << getName() << " added a comment to " << c->getCourseName() << endl;
    c->post(sender, msg);
}

// Default comment method where the student is the sender
void Student::comment(Course* c, const string& msg) { 
    comment(c, msg, this); 
}

void Student::addGrade(double score) {
    grades.add(score);
}
