#pragma once
#include "User.h"
#include <vector>

class Course;

class Teacher : public User {
private:
    std::string subject;
    std::vector<Course*> managedCourses;
public:
    Teacher(const std::string& n, int i, const std::string& s);
    void addCourse(Course* c);
    void displayDashboard() const override;
    void announce(Course* c, const std::string& msg, IObserver* sender);
    void announce(Course* c, const std::string& msg);
    void setSubject(const std::string& s);
    const std::string& getSubject() const;
    const std::vector<Course*>& getManagedCourses() const { return managedCourses; }
};