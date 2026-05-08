#pragma once
#include <vector>
#include <memory>
#include "Student.h"
#include "Teacher.h"
#include "Course.h"
#include "FileIO.h"

class LMS {
    std::vector<std::unique_ptr<Student>> students;
    std::vector<std::unique_ptr<Teacher>> teachers;
    std::vector<std::unique_ptr<Course>> courses;

    Student* findStudentById(int id);
    Teacher* findTeacherById(int id);
    Course* findCourseByName(const std::string& name);

public:
    void saveAll();
    void loadAll();
    void run();
};