#include "User.h"
#include <iostream>
using namespace std;

int User::totalUsers = 0;

User::User(const string& n, int i) : name(n), id(i) { totalUsers++; }

void User::update(IObserver* sender, const string& message) {
    if (this != sender)
        cout << "Notification to " << name << " (ID: " << id << "): " << message << endl;
}

int User::getTotalUsers() { return totalUsers; }
const string& User::getName() const { return name; }
int User::getID() const { return id; }