#pragma once
#include "User.h"

class Course;

class Student : public User {
private:
    float cgpa;
public:
    Student(const std::string& n, int i, float c);
    void displayDashboard() const override;
    void setCGPA(float n);
    float getCGPA() const;
    void comment(Course* c, const std::string& msg, IObserver* sender);
    void comment(Course* c, const std::string& msg);
};