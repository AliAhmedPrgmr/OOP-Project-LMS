#pragma once
#include "User.h"

class SystemAdmin : public User {
public:
    SystemAdmin(const std::string& n, int i);
    void displayDashboard() const override;
    // Additional admin methods can be added here
};