#include "Teacher.h"
#include "Course.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;

// Constructor: Initializes the teacher and validates their subject specialization
Teacher::Teacher(const string& n, int i, const string& s) : User(n, i), subject(s) {
    if (s.empty()) {
        throw InvalidInputException("Teacher subject cannot be empty.");
    }
    cout << "Success: " << n << " is registered as a teacher!" << endl;
}

// Displays teacher info and a list of all courses they manage
void Teacher::displayDashboard() const {
    cout << "\n==== Teacher Portal ====" << endl;
    cout << "Teacher: " << getName() << " | ID: " << getID() << " | Subject: " << subject << endl;
    cout << "Managing " << managedCourses.size() << " course(s):" << endl;
    
    if (managedCourses.empty()) {
        cout << "(No courses assigned yet)" << endl;
    } else {
        for (const auto* course : managedCourses) {
            if (course) {
                cout << " * " << course->getCourseName() << endl;
            }
        }
    }
    cout << "=========================" << endl;
}

// Overloaded announce method: sends a teacher announcement to the course
void Teacher::announce(Course* c, const string& msg, IObserver* sender) {
    if (!c) {
        throw InvalidInputException("Announcement failed: Target course cannot be null.");
    }
    if (msg.empty()) {
        throw InvalidInputException("Announcement failed: Message cannot be empty.");
    }
    
    cout << "[Teacher Log] " << getName() << " posted an announcement in " << c->getCourseName() << endl;
    c->post(sender, msg);
}

// Default announce method where the teacher is the sender
void Teacher::announce(Course* c, const string& msg) { 
    announce(c, msg, this); 
}

// Links a course to the teacher and updates the course's teacher list
void Teacher::addCourse(Course* c) {
    if (!c) {
        throw InvalidInputException("Action failed: Cannot add a null course.");
    }
    
    // Ensure the teacher is not already assigned to this course
    for (auto* crs : managedCourses) {
        if (crs == c) {
            throw DuplicateEntityException("The course '" + c->getCourseName() + "' is already assigned to this teacher.");
        }
    }
    
    managedCourses.push_back(c);
    c->addTeacher(this); // Update the Course side of the relationship
}

// Setters and Getters
void Teacher::setSubject(const string& s) { 
    if (s.empty()) throw InvalidInputException("Subject cannot be empty.");
    subject = s; 
}

const string& Teacher::getSubject() const { 
    return subject; 
}