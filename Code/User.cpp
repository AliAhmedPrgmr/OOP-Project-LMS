#include "User.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;

// Initialize the static member to track total users across the system
int User::totalUsers = 0;

// Constructor with validation logic
User::User(const string& n, int i) : name(n), id(i) {
    if (n.empty()) {
        throw InvalidInputException("User name cannot be empty.");
    }
    if (i < 0) {
        throw InvalidInputException("User ID must be non-negative.");
    }
    
    totalUsers++;
}

// Observer pattern update method: prevents users from notifying themselves
void User::update(IObserver* sender, const string& message) {
    if (this != sender) {
        cout << "Notification to " << name << " (ID: " << id << "): " << message << endl;
    }
}

// Getter for total number of users created
int User::getTotalUsers() { 
    return totalUsers; 
}

// Getter for user name
const string& User::getName() const { 
    return name; 
}

// Getter for user ID
int User::getID() const { 
    return id; 
}