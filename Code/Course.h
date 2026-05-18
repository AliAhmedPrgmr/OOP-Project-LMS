#pragma once
#include <memory>
#include <string>
#include <vector>
#include "IObserver.h"
#include "Exceptions.h" // Added to support Exception Handling

class Student;
class Teacher;

class Course {
private:
    std::string courseName;
    std::vector<Student*> studentList;
    std::vector<Teacher*> teacherList;
    std::vector<IObserver*> observers;

public:
    // Constructor handles basic validation through Exceptions
    Course(const std::string& n);
    
    const std::string& getCourseName() const;
    
    // Core methods with pointer safety
    void enrollStudent(Student* s);
    void addTeacher(Teacher* t);
    
    void displayStudentList() const;
    void displayTeacherList() const;
    
    // Part of the Observer Pattern: notifies all attached observers
    void post(IObserver* sender, const std::string& msg);

    const std::vector<Student*>& getStudentList() const { return studentList; }
    const std::vector<Teacher*>& getTeacherList() const { return teacherList; }

    // Friend functions for persistence (File Handling)
    friend void saveCoursesToFile(const std::vector<std::unique_ptr<Course>>& courses, const std::string& filename);
    friend void loadCoursesFromFile(std::vector<std::unique_ptr<Course>>& courses,
                                    const std::vector<std::unique_ptr<Teacher>>& teachers,
                                    const std::vector<std::unique_ptr<Student>>& students,
                                    const std::string& filename);
};