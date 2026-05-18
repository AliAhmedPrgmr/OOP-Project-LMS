#pragma once
#include "User.h"
#include "DynamicArray.h"
class Course;

class Student : virtual public User {
private:
    float cgpa;
    DynamicArray<double> grades;
public:
    Student(const std::string& n, int i, float c);
    void displayDashboard() const override;
    void setCGPA(float n);
    float getCGPA() const;
    void addGrade(double score); 
    void comment(Course* c, const std::string& msg, IObserver* sender);
    void comment(Course* c, const std::string& msg);
};
