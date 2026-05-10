#include "UserFactory.h"
#include <iostream>
#include <limits>

using namespace std;

namespace {
constexpr float DEFAULT_TA_CGPA = 0.0f;
}

// Static member initialization
bool UserFactory::defaultsRegistered = false;

// Accessor for the static registry map (ensures it is initialized before use)
map<string, UserFactory::Creator>& UserFactory::getRegistry() {
    static map<string, Creator> registry;
    return registry;
}

// Ensures default types (Student/Teacher) are registered once
void UserFactory::ensureDefaultCreators() {
    if (!defaultsRegistered) {
        registerDefaultCreators();
    }
}

// OCP Core: Register new user types without changing this class
bool UserFactory::registerCreator(const string& userType, Creator creator) {
    ensureDefaultCreators();
    auto& registry = getRegistry();
    return registry.emplace(userType, std::move(creator)).second;
}

// Automatically registers the default types using lambdas
void UserFactory::registerDefaultCreators() {
    auto& registry = getRegistry();
    registry["student"] = []() { return createStudentInteractive(); };
    registry["teacher"] = []() { return createTeacherInteractive(); };
    registry["admin"] = []() { return createSystemAdminInteractive(); };
    defaultsRegistered = true;
}

// Creates a user based on type string, throwing an exception if type is unknown
unique_ptr<User> UserFactory::createUserInteractive(const string& userType) {
    ensureDefaultCreators();
    auto& registry = getRegistry();
    auto it = registry.find(userType);
    if (it == registry.end()) {
        throw InvalidInputException("Unsupported user type: " + userType);
    }
    return it->second();
}

// Static helper to create a Student with explicit validation
unique_ptr<Student> UserFactory::createStudent(const string& name, int id, float cgpa) {
    if (name.empty()) throw InvalidInputException("Student name cannot be empty.");
    if (id < 0) throw InvalidInputException("Student ID must be non-negative.");
    if (cgpa < 0.0f || cgpa > 4.0f) throw InvalidInputException("Student CGPA must be between 0 and 4.");
    
    return make_unique<Student>(name, id, cgpa);
}

// Static helper to create a SystemAdmin with explicit validation
std::unique_ptr<SystemAdmin> UserFactory::createSystemAdmin(const std::string& name, int id) {
    if (name.empty()) throw InvalidInputException("SystemAdmin name cannot be empty.");
    if (id < 0) throw InvalidInputException("SystemAdmin ID must be non-negative.");
    
    return std::make_unique<SystemAdmin>(name, id);
}

// Static helper to create a Teacher with explicit validation
std::unique_ptr<Teacher> UserFactory::createTeacher(const std::string& name, int id, const std::string& subject) {
    if (name.empty()) throw InvalidInputException("Teacher name cannot be empty.");
    if (id < 0) throw InvalidInputException("Teacher ID must be non-negative.");
    if (subject.empty()) throw InvalidInputException("Teacher subject cannot be empty.");
    
    return std::make_unique<Teacher>(name, id, subject);
}

// Interactive Student creation with robust input validation
unique_ptr<Student> UserFactory::createStudentInteractive() {
    string name;
    int id;
    float cgpa;

    cout << "\n--- Student Creation ---" << endl;
    cout << "Enter Student Name: ";
    cin >> ws;
    getline(cin, name);

    while (true) {
        cout << "Enter Student ID: ";
        if (cin >> id) break;
        cout << "  [Error] Invalid ID. Please enter an integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    while (true) {
        cout << "Enter Student CGPA (0.0 - 4.0): ";
        if (cin >> cgpa && cgpa >= 0.0f && cgpa <= 4.0f) break;
        cout << "  [Error] Invalid CGPA. Must be between 0 and 4: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return createStudent(name, id, cgpa);
}

// Interactive Teacher creation with robust input validation
unique_ptr<Teacher> UserFactory::createTeacherInteractive() {
    string name;
    string subject;
    int id;

    cout << "\n--- Teacher Registration ---" << endl;
    cout << "Enter Teacher Name: ";
    cin >> ws;
    getline(cin, name);

    while (true) {
        cout << "Enter Teacher ID: ";
        if (cin >> id) break;
        cout << "  [Error] Invalid ID. Please enter an integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter Teaching Subject: ";
    cin >> ws;
    getline(cin, subject);

    return createTeacher(name, id, subject);
}

// Interactive TeachingAssistant creation with robust input validation
std::unique_ptr<TeachingAssistant> UserFactory::createTAInteractive() {
    string name;
    string email;
    int id;

    cout << "\n--- Teaching Assistant Registration ---" << endl;
    cout << "Enter TA Name: ";
    cin >> ws;
    getline(cin, name);

    while (true) {
        cout << "Enter TA ID: ";
        if (cin >> id) break;
        cout << "  [Error] Invalid ID. Please enter an integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter TA Email: ";
    getline(cin, email);

    return make_unique<TeachingAssistant>(name, id, DEFAULT_TA_CGPA, email);
}

// Interactive SystemAdmin creation with robust input validation
std::unique_ptr<SystemAdmin> UserFactory::createSystemAdminInteractive() {
    std::string name;
    int id;

    std::cout << "\n--- System Admin Creation ---" << std::endl;
    std::cout << "Enter Admin Name: ";
    std::cin >> std::ws;
    std::getline(std::cin, name);

    while (true) {
        std::cout << "Enter Admin ID: ";
        if (std::cin >> id) break;
        std::cout << "  [Error] Invalid ID. Please enter an integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return createSystemAdmin(name, id);
}
