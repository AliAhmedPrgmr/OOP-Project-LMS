#include "SystemAdmin.h"
#include <iostream>

using namespace std;

SystemAdmin::SystemAdmin(const string& n, int i) : User(n, i) {
    cout << "Success: " << n << " is registered as a System Admin!" << endl;
}

void SystemAdmin::displayDashboard() const {
    cout << "\n==== System Admin Portal ====" << endl;
    cout << "Admin: " << getName() << " | ID: " << getID() << endl;
    cout << "Total Users in System: " << getTotalUsers() << endl;
    cout << "=============================" << endl;
}