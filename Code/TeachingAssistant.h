#pragma once
#include "Student.h"
#include "Teacher.h"
#include <vector>

class Course;

class TeachingAssistant : public Student, public Teacher {
private:
    std::string email;
    std::vector<std::string> assistantTasks;
    float rating;

public:
    TeachingAssistant(const std::string& n, int i, float c, const std::string& e);
    const std::string& getEmail() const;
    void displayDashboard() const override;
    
    void assignTask(const std::string& task);
    void viewAssignedTasks() const;
    void gradeStudent(const std::string& studentName, double score, const std::string& feedback);
    void conductLabSession(Course* c, const std::string& sessionDetails);
    void setRating(float r);
    float getRating() const { return rating; }
    const std::vector<std::string>& getTasks() const { return assistantTasks; }
};