#pragma once
#include <functional>
#include <map>
#include <memory>
#include <string>
#include "Exceptions.h"
#include "Student.h"
#include "Teacher.h"
#include "SystemAdmin.h"

/**
 * UserFactory implements the Open/Closed Principle (OCP).
 * New user types can be registered without modifying the factory's core logic.
 */
class UserFactory {
public:
    // Definition for a creator function that returns a unique_ptr to a User
    using Creator = std::function<std::unique_ptr<User>()>;

    // Registers a new user type creator in the registry (OCP Core)
    static bool registerCreator(const std::string& userType, Creator creator);
    
    // Creates a user by asking for input through the console based on registered types
    static std::unique_ptr<User> createUserInteractive(const std::string& userType);

    // Specific interactive methods for the original types
    static std::unique_ptr<Student> createStudentInteractive();
    static std::unique_ptr<Teacher> createTeacherInteractive();
    static std::unique_ptr<SystemAdmin> createSystemAdminInteractive();

    // Direct creation methods with validation (Exception Handling)
    static std::unique_ptr<Student> createStudent(const std::string& name, int id, float cgpa);
    static std::unique_ptr<Teacher> createTeacher(const std::string& name, int id, const std::string& subject);
    static std::unique_ptr<SystemAdmin> createSystemAdmin(const std::string& name, int id);

private:
    // Static registry map to store the creators
    static std::map<std::string, Creator>& getRegistry();
    
    // Safety flags to ensure Student and Teacher are always available
    static bool defaultsRegistered;
    static void ensureDefaultCreators();
    static void registerDefaultCreators();
};