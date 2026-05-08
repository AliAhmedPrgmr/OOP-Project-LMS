#include "LMS.h"
#include "UserFactory.h"
#include <iostream>
#include <limits>
using namespace std;

Student* LMS::findStudentById(int id) {
    for (auto& s : students) if (s->getID() == id) return s.get();
    return nullptr;
}
Teacher* LMS::findTeacherById(int id) {
    for (auto& t : teachers) if (t->getID() == id) return t.get();
    return nullptr;
}
Course* LMS::findCourseByName(const string& name) {
    for (auto& c : courses) if (c->getCourseName() == name) return c.get();
    return nullptr;
}

void LMS::saveAll() {
    saveStudentsToFile(students, "students.txt");
    saveTeachersToFile(teachers, "teachers.txt");
    saveCoursesToFile(courses, "courses.txt");
}

void LMS::loadAll() {
    loadStudentsFromFile(students, "students.txt");
    loadTeachersFromFile(teachers, "teachers.txt");
    loadCoursesFromFile(courses, teachers, students, "courses.txt");
}

void LMS::run() {
    while (true) {
        cout << "\n===== LMS MENU =====\n"
             << "1) Create Student\n2) Create Teacher\n3) Create Course\n"
             << "4) Assign Course to Teacher\n5) Enroll Student in Course\n"
             << "6) Teacher Announcement\n7) Student Comment\n8) List Courses\n"
             << "9) List Students in Course\n10) List Teachers in Course\n0) Exit\nChoose: ";

        int choice;
        if (!(cin >> choice)) {
            cout << "Please enter a valid menu number.\n";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) break;
        else if (choice == 1) students.push_back(UserFactory::createStudentInteractive());
        else if (choice == 2) teachers.push_back(UserFactory::createTeacherInteractive());
        else if (choice == 3) {
            string cname;
            cout << "Course name: "; getline(cin, cname);
            if (!cname.empty() && !findCourseByName(cname))
                courses.push_back(make_unique<Course>(cname));
            else cout << "Invalid or duplicate course name.\n";
        }
        else if (choice == 4) {
            int tid; string cname;
            cout << "Teacher id: "; cin >> tid; cin.ignore();
            cout << "Course name: "; getline(cin, cname);
            Teacher* t = findTeacherById(tid); Course* c = findCourseByName(cname);
            if (!t || !c) { cout << "Teacher or course not found.\n"; continue; }
            t->addCourse(c);
        }
        else if (choice == 5) {
            int sid; string cname;
            cout << "Student id: "; cin >> sid; cin.ignore();
            cout << "Course name: "; getline(cin, cname);
            Student* s = findStudentById(sid); Course* c = findCourseByName(cname);
            if (!s || !c) { cout << "Student or course not found.\n"; continue; }
            c->enrollStudent(s);
        }
        else if (choice == 6) {
            int tid; string cname, msg;
            cout << "Teacher id: "; cin >> tid; cin.ignore();
            cout << "Course name: "; getline(cin, cname);
            cout << "Announcement: "; getline(cin, msg);
            Teacher* t = findTeacherById(tid); Course* c = findCourseByName(cname);
            if (!t || !c) { cout << "Teacher or course not found.\n"; continue; }
            t->announce(c, msg);
        }
        else if (choice == 7) {
            int sid; string cname, msg;
            cout << "Student id: "; cin >> sid; cin.ignore();
            cout << "Course name: "; getline(cin, cname);
            cout << "Comment: "; getline(cin, msg);
            Student* s = findStudentById(sid); Course* c = findCourseByName(cname);
            if (!s || !c) { cout << "Student or course not found.\n"; continue; }
            s->comment(c, msg);
        }
        else if (choice == 8) {
            cout << "\nCourses:\n";
            for (const auto& c : courses) cout << "- " << c->getCourseName() << endl;
        }
        else if (choice == 9) {
            string cname; cout << "Course name: "; getline(cin, cname);
            Course* c = findCourseByName(cname);
            if (!c) { cout << "Course not found.\n"; continue; }
            c->displayStudentList();
        }
        else if (choice == 10) {
            string cname; cout << "Course name: "; getline(cin, cname);
            Course* c = findCourseByName(cname);
            if (!c) { cout << "Course not found.\n"; continue; }
            c->displayTeacherList();
        }
        else cout << "Invalid choice.\n";
    }
}