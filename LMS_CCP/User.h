#pragma once
#include <string>
#include "IObserver.h"

class User : public IObserver {
private:
    static int totalUsers;
    std::string name;
    const int id;
public:
    User(const std::string& n, int i);
    void update(IObserver* sender, const std::string& message) override;
    virtual void displayDashboard() const = 0;
    static int getTotalUsers();
    const std::string& getName() const;
    int getID() const;
    virtual ~User() {}
};