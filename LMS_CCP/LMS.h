#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Student.h"
#include "Teacher.h"
#include "TeachingAssistant.h"
#include "SystemAdmin.h"
#include "Course.h"
#include "FileIO.h"
#include "Exceptions.h" // Required for error handling

/**
 * LMS Class implemented as a Singleton.
 * Manages the core data and application lifecycle.
 */
class LMS {
private:
    // Core data storage using smart pointers
    std::vector<std::unique_ptr<Student>> students;
    std::vector<std::unique_ptr<Teacher>> teachers;
    std::vector<std::unique_ptr<TeachingAssistant>> tas;
    std::vector<std::unique_ptr<SystemAdmin>> admins;
    std::vector<std::unique_ptr<Course>> courses;

    // Private helper methods for internal lookup
    Student* findStudentById(int id);
    Teacher* findTeacherById(int id);
    SystemAdmin* findSystemAdminById(int id);
    Course* findCourseByName(const std::string& name);

    // Singleton Pattern: Private constructor prevents manual instantiation
    LMS() = default;

    // Static instance pointer - This is a DECLARATION only
    static LMS* instance;

public:
    // Singleton Pattern: Delete copy constructor and assignment operator
    LMS(const LMS&) = delete;
    LMS& operator=(const LMS&) = delete;

    // Global access point for the LMS instance
    static LMS& getInstance();

    // Core application lifecycle methods
    void saveAll();
    void loadAll();
    void run();

    // Methods to add entities with validation/exception handling
    void addStudent(std::unique_ptr<Student> student);
    void addTeacher(std::unique_ptr<Teacher> teacher);
    void addTA(std::unique_ptr<TeachingAssistant> ta);
    void addSystemAdmin(std::unique_ptr<SystemAdmin> admin);
    void addCourse(std::unique_ptr<Course> course);
};
