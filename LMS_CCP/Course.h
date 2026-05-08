#pragma once
#include <string>
#include <vector>
#include "IObserver.h"

class Student;
class Teacher;

class Course {
private:
    std::string courseName;
    std::vector<Student*> studentList;
    std::vector<Teacher*> teacherList;
    std::vector<IObserver*> observers;

public:
    Course(const std::string& n);
    const std::string& getCourseName() const;
    void enrollStudent(Student* s);
    void addTeacher(Teacher* t);
    void displayStudentList() const;
    void displayTeacherList() const;
    void post(IObserver* sender, const std::string& msg);

    friend void saveCoursesToFile(const std::vector<std::unique_ptr<Course>>& courses, const std::string& filename);
    friend void loadCoursesFromFile(std::vector<std::unique_ptr<Course>>& courses,
                                    const std::vector<std::unique_ptr<Teacher>>& teachers,
                                    const std::vector<std::unique_ptr<Student>>& students,
                                    const std::string& filename);
};