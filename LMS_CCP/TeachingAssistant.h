#pragma once
#include "Student.h"
#include "Teacher.h"

class TeachingAssistant : public Student, public Teacher {
public:
    TeachingAssistant(const std::string& n, int i, float c, const std::string& s);
    void displayDashboard() const override;
};
