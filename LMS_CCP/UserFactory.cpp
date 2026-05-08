#include "UserFactory.h"
#include <iostream>
#include <limits>
using namespace std;

unique_ptr<Student> UserFactory::createStudentInteractive() {
    string name; int id; float cgpa;
    cout << "Student name: "; cin >> ws; getline(cin, name);
    while (true) {
        cout << "Student id: ";
        if (cin >> id) break;
        cout << "  Invalid input. Enter an integer: ";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    while (true) {
        cout << "Student CGPA (0-4): ";
        if (cin >> cgpa && cgpa >= 0 && cgpa <= 4) break;
        cout << "  Invalid CGPA. Must be between 0 and 4." << endl;
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return make_unique<Student>(name, id, cgpa);
}

unique_ptr<Teacher> UserFactory::createTeacherInteractive() {
    string name, subject; int id;
    cout << "Teacher name: "; cin >> ws; getline(cin, name);
    while (true) {
        cout << "Teacher id: ";
        if (cin >> id) break;
        cout << "  Invalid input. Enter an integer: ";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Teacher subject: "; cin >> ws; getline(cin, subject);
    return make_unique<Teacher>(name, id, subject);
}