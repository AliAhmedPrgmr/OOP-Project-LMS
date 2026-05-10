#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Student.h"
#include "Teacher.h"
#include "SystemAdmin.h"
#include "Course.h"

/**
 * FileIO provides static functions to handle data persistence for the LMS.
 * It supports saving and loading Students, Teachers, and Courses with their relationships.
 */

// --- Student Persistence ---
void saveStudentsToFile(const std::vector<std::unique_ptr<Student>>& students, const std::string& filename);
void loadStudentsFromFile(std::vector<std::unique_ptr<Student>>& students, const std::string& filename);

// --- Teacher Persistence ---
void saveTeachersToFile(const std::vector<std::unique_ptr<Teacher>>& teachers, const std::string& filename);
void loadTeachersFromFile(std::vector<std::unique_ptr<Teacher>>& teachers, const std::string& filename);

// --- SystemAdmin Persistence ---
void saveSystemAdminsToFile(const std::vector<std::unique_ptr<SystemAdmin>>& admins, const std::string& filename);
void loadSystemAdminsFromFile(std::vector<std::unique_ptr<SystemAdmin>>& admins, const std::string& filename);

// --- Course Persistence ---
// Note: Loading courses requires references to existing teachers and students to re-establish links.
void saveCoursesToFile(const std::vector<std::unique_ptr<Course>>& courses, const std::string& filename);
void loadCoursesFromFile(std::vector<std::unique_ptr<Course>>& courses,
                         const std::vector<std::unique_ptr<Teacher>>& teachers,
                         const std::vector<std::unique_ptr<Student>>& students,
                         const std::string& filename);