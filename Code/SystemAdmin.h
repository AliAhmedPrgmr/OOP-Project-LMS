#pragma once
#include "User.h"
#include <vector>
#include <string>

class Course;

class SystemAdmin : public User {
private:
    std::vector<std::string> systemLogs;
    std::vector<Course*> supervisedCourses;
    
public:
    SystemAdmin(const std::string& n, int i);
    void displayDashboard() const override;
    
    void addLog(const std::string& action);
    void viewSystemLogs() const;
    void generateSystemReport() const;
    void deleteUser(const std::string& userType, int userId);
    
    void superviseCourse(Course* c);
    void viewSupervisedCourses() const;
    
    const std::vector<std::string>& getLogs() const { return systemLogs; }
    const std::vector<Course*>& getSupervisedCourses() const { return supervisedCourses; }
};