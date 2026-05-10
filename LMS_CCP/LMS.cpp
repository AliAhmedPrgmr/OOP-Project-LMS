#include "LMS.h"
#include "UserFactory.h"
#include <iostream>
#include <limits>

using namespace std;

// Static instance initialization
LMS* LMS::instance = nullptr;

// ==========================================
// LMS Implementation
// ==========================================

LMS& LMS::getInstance() {
    if (!instance) {
        instance = new LMS();
    }
    return *instance;
}

Student* LMS::findStudentById(int id) {
    for (auto& s : students) 
        if (s->getID() == id) return s.get();
    return nullptr;
}

Teacher* LMS::findTeacherById(int id) {
    for (auto& t : teachers) 
        if (t->getID() == id) return t.get();
    return nullptr;
}

SystemAdmin* LMS::findSystemAdminById(int id) {
    for (auto& a : admins) 
        if (a->getID() == id) return a.get();
    return nullptr;
}

Course* LMS::findCourseByName(const string& name) {
    for (auto& c : courses) 
        if (c->getCourseName() == name) return c.get();
    return nullptr;
}

void LMS::addStudent(unique_ptr<Student> student) {
    if (student) {
        if (findStudentById(student->getID())) {
            throw DuplicateEntityException("Student with ID " + std::to_string(student->getID()) + " already exists.");
        }
        students.push_back(move(student));
    }
}

void LMS::addTeacher(unique_ptr<Teacher> teacher) {
    if (teacher) {
        if (findTeacherById(teacher->getID())) {
            throw DuplicateEntityException("Teacher with ID " + std::to_string(teacher->getID()) + " already exists.");
        }
        teachers.push_back(move(teacher));
    }
}

void LMS::addSystemAdmin(unique_ptr<SystemAdmin> admin) {
    if (admin) {
        if (findSystemAdminById(admin->getID())) {
            throw DuplicateEntityException("SystemAdmin with ID " + std::to_string(admin->getID()) + " already exists.");
        }
        admins.push_back(move(admin));
    }
}

void LMS::addCourse(unique_ptr<Course> course) {
    if (course) {
        if (findCourseByName(course->getCourseName())) {
            throw DuplicateEntityException("Course with name '" + course->getCourseName() + "' already exists.");
        }
        courses.push_back(move(course));
    }
}

void LMS::saveAll() {
    saveStudentsToFile(students, "students.txt");
    saveTeachersToFile(teachers, "teachers.txt");
    saveSystemAdminsToFile(admins, "admins.txt");
    saveCoursesToFile(courses, "courses.txt");
}

void LMS::loadAll() {
    // Clear existing data to avoid duplicates
    students.clear();
    teachers.clear();
    admins.clear();
    courses.clear();
    
    loadStudentsFromFile(students, "students.txt");
    loadTeachersFromFile(teachers, "teachers.txt");
    loadSystemAdminsFromFile(admins, "admins.txt");
    loadCoursesFromFile(courses, teachers, students, "courses.txt");
}

void LMS::run() {
    while (true) {
        cout << "\n===== LMS MENU =====\n"
             << "1) Create Student\n"
             << "2) Create Teacher\n"
             << "3) Create System Admin\n"
             << "4) Create Course\n"
             << "5) Assign Course to Teacher\n"
             << "6) Enroll Student in Course\n"
             << "7) Teacher Announcement\n"
             << "8) Student Comment\n"
             << "9) List Courses\n"
             << "10) List Students in Course\n"
             << "11) List Teachers in Course\n"
             << "0) Exit\nChoice: ";

        int choice;
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) break;

        if (choice == 1) {
            auto student = UserFactory::createStudentInteractive();
            if (student) addStudent(move(student));
        }
        else if (choice == 2) {
            auto teacher = UserFactory::createTeacherInteractive();
            if (teacher) addTeacher(move(teacher));
        }
        else if (choice == 3) {
            auto admin = UserFactory::createSystemAdminInteractive();
            if (admin) addSystemAdmin(move(admin));
        }
        else if (choice == 4) {
            string cname;
            cout << "Course name: "; 
            getline(cin, cname);
            if (!cname.empty() && !findCourseByName(cname)) {
                try {
                    addCourse(make_unique<Course>(cname));
                    cout << "Created: " << cname << "\n";
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << "\n";
                }
            } else {
                cout << "Invalid or duplicate course name.\n";
            }
        }
        else if (choice == 5) {
            int tid; 
            string cname;
            cout << "Teacher ID: "; 
            cin >> tid; 
            cin.ignore();
            cout << "Course name: "; 
            getline(cin, cname);
            
            Teacher* t = findTeacherById(tid);
            Course* c = findCourseByName(cname);
            if (!t || !c) { 
                cout << "Not found.\n"; 
                continue; 
            }
            t->addCourse(c);
        }
        else if (choice == 6) {
            int sid; 
            string cname;
            cout << "Student ID: "; 
            cin >> sid; 
            cin.ignore();
            cout << "Course name: "; 
            getline(cin, cname);
            
            Student* s = findStudentById(sid);
            Course* c = findCourseByName(cname);
            if (!s || !c) { 
                cout << "Not found.\n"; 
                continue; 
            }
            c->enrollStudent(s);
        }
        else if (choice == 7) {
            int tid; 
            string cname, msg;
            cout << "Teacher ID: "; 
            cin >> tid; 
            cin.ignore();
            cout << "Course name: "; 
            getline(cin, cname);
            cout << "Message: "; 
            getline(cin, msg);
            
            Teacher* t = findTeacherById(tid);
            Course* c = findCourseByName(cname);
            if (!t || !c) { 
                cout << "Not found.\n"; 
                continue; 
            }
            // Check if teacher is assigned to the course
            bool isAssigned = false;
            for (auto* managed : t->getManagedCourses()) {
                if (managed == c) {
                    isAssigned = true;
                    break;
                }
            }
            if (!isAssigned) {
                cout << "Teacher is not assigned to this course.\n";
                continue;
            }
            t->announce(c, msg);
        }
        else if (choice == 8) {
            int sid; 
            string cname, msg;
            cout << "Student ID: "; 
            cin >> sid; 
            cin.ignore();
            cout << "Course name: "; 
            getline(cin, cname);
            cout << "Message: "; 
            getline(cin, msg);
            
            Student* s = findStudentById(sid);
            Course* c = findCourseByName(cname);
            if (!s || !c) { 
                cout << "Not found.\n"; 
                continue; 
            }
            // Check if student is enrolled in the course
            bool isEnrolled = false;
            for (auto* enrolled : c->getStudentList()) {
                if (enrolled == s) {
                    isEnrolled = true;
                    break;
                }
            }
            if (!isEnrolled) {
                cout << "Student is not enrolled in this course.\n";
                continue;
            }
            s->comment(c, msg);
        }
        else if (choice == 9) {
            if (courses.empty()) {
                cout << "No courses.\n";
            } else {
                cout << "\nCourses:\n";
                for (const auto& c : courses) 
                    cout << "  - " << c->getCourseName() << endl;
            }
        }
        else if (choice == 10) {
            string cname;
            cout << "Course name: "; 
            getline(cin, cname);
            Course* c = findCourseByName(cname);
            if (!c) { 
                cout << "Not found.\n"; 
                continue; 
            }
            c->displayStudentList();
        }
        else if (choice == 11) {
            string cname;
            cout << "Course name: "; 
            getline(cin, cname);
            Course* c = findCourseByName(cname);
            if (!c) { 
                cout << "Not found.\n"; 
                continue; 
            }
            c->displayTeacherList();
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
}
