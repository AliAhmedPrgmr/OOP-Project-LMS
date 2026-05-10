#pragma once
#include "Student.h"
#include "Teacher.h"

class TeachingAssistant : public Student, public Teacher {
private:
    std::string email;

public:
    TeachingAssistant(const std::string& n, int i, float c, const std::string& e);
    const std::string& getEmail() const;
    void displayDashboard() const override;
};
