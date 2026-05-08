#pragma once
#include <memory>
#include "Student.h"
#include "Teacher.h"

class UserFactory {
public:
    static std::unique_ptr<Student> createStudentInteractive();
    static std::unique_ptr<Teacher> createTeacherInteractive();
};