#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Student.h"
#include "Teacher.h"
#include "Course.h"

void saveStudentsToFile(const std::vector<std::unique_ptr<Student>>& students, const std::string& filename);
void loadStudentsFromFile(std::vector<std::unique_ptr<Student>>& students, const std::string& filename);

void saveTeachersToFile(const std::vector<std::unique_ptr<Teacher>>& teachers, const std::string& filename);
void loadTeachersFromFile(std::vector<std::unique_ptr<Teacher>>& teachers, const std::string& filename);

void saveCoursesToFile(const std::vector<std::unique_ptr<Course>>& courses, const std::string& filename);
void loadCoursesFromFile(std::vector<std::unique_ptr<Course>>& courses,
                         const std::vector<std::unique_ptr<Teacher>>& teachers,
                         const std::vector<std::unique_ptr<Student>>& students,
                         const std::string& filename);