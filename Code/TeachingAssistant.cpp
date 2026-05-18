#include "TeachingAssistant.h"
#include "Course.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>

using namespace std;

TeachingAssistant::TeachingAssistant(const string& n, int i, float c, const string& e)
    : User(n, i), Student(n, i, c), Teacher(n, i, "Teaching Assistant"), email(e), rating(0.0f) {
    if (email.empty()) {
        throw InvalidInputException("Teaching assistant email cannot be empty.");
    }
    cout << "Success: " << n << " is registered as a teaching assistant!" << endl;
}

const string& TeachingAssistant::getEmail() const {
    return email;
}

void TeachingAssistant::displayDashboard() const {
    cout << "\n========== Teaching Assistant Portal ==========" << endl;
    cout << "Name: " << getName() << endl;
    cout << "ID: " << getID() << endl;
    cout << "CGPA: " << getCGPA() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Performance Rating: " << fixed << setprecision(2) << rating << "/5.0" << endl;
    cout << "Assigned Tasks: " << assistantTasks.size() << endl;
    cout << "Courses Teaching: " << getManagedCourses().size() << endl;
    cout << "==============================================" << endl;
}

void TeachingAssistant::assignTask(const std::string& task) {
    if (task.empty()) {
        throw InvalidInputException("Task description cannot be empty.");
    }
    assistantTasks.push_back(task);
    cout << "[TA Task] Task assigned to " << getName() << ": " << task << endl;
}

void TeachingAssistant::viewAssignedTasks() const {
    if (assistantTasks.empty()) {
        cout << "\n[INFO] No tasks assigned to " << getName() << ".\n" << endl;
        return;
    }
    
    cout << "\n========== Assigned Tasks for " << getName() << " ==========" << endl;
    for (size_t i = 0; i < assistantTasks.size(); ++i) {
        cout << (i + 1) << ". " << assistantTasks[i] << endl;
    }
    cout << "================================================" << endl;
}

void TeachingAssistant::gradeStudent(const std::string& studentName, double score, const std::string& feedback) {
    if (score < 0.0 || score > 100.0) {
        throw InvalidInputException("Score must be between 0 and 100.");
    }
    if (feedback.empty()) {
        throw InvalidInputException("Feedback cannot be empty.");
    }
    
    cout << "\n========== Grading Report ==========" << endl;
    cout << "Graded by: " << getName() << " (TA)" << endl;
    cout << "Student: " << studentName << endl;
    cout << "Score: " << fixed << setprecision(2) << score << "/100" << endl;
    cout << "Grade: ";
    
    if (score >= 90) cout << "A";
    else if (score >= 80) cout << "B";
    else if (score >= 70) cout << "C";
    else if (score >= 60) cout << "D";
    else cout << "F";
    
    cout << "\nFeedback: " << feedback << endl;
    cout << "====================================" << endl;
}

void TeachingAssistant::conductLabSession(Course* c, const std::string& sessionDetails) {
    if (!c) {
        throw InvalidInputException("Cannot conduct lab session: Course cannot be null.");
    }
    if (sessionDetails.empty()) {
        throw InvalidInputException("Session details cannot be empty.");
    }
    
    bool isAssigned = false;
    for (auto* managed : getManagedCourses()) {
        if (managed == c) {
            isAssigned = true;
            break;
        }
    }
    
    if (!isAssigned) {
        throw InvalidOperationException("TA is not assigned to teach this course.");
    }
    
    cout << "\n========== Lab Session Started ==========" << endl;
    cout << "Conducted by: " << getName() << " (TA)" << endl;
    cout << "Course: " << c->getCourseName() << endl;
    cout << "Session Details: " << sessionDetails << endl;
    cout << "Number of Students: " << c->getStudentList().size() << endl;
    cout << "========================================" << endl;
}

void TeachingAssistant::setRating(float r) {
    if (r < 0.0f || r > 5.0f) {
        throw InvalidInputException("Rating must be between 0.0 and 5.0.");
    }
    rating = r;
    cout << "[Rating] " << getName() << "'s performance rating updated to " 
         << fixed << setprecision(2) << r << "/5.0" << endl;
}