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

TeachingAssistant* LMS::findTAById(int id) {
    for (auto& ta : tas)
        if (ta->getID() == id) return ta.get();
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

void LMS::addTA(unique_ptr<TeachingAssistant> ta) {
    if (ta) {
        if (findTAById(ta->getID())) {
            throw DuplicateEntityException("TeachingAssistant with ID " + std::to_string(ta->getID()) + " already exists.");
        }
        tas.push_back(move(ta));
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
    tas.clear();
    admins.clear();
    courses.clear();
    
    loadStudentsFromFile(students, "students.txt");
    loadTeachersFromFile(teachers, "teachers.txt");
    loadSystemAdminsFromFile(admins, "admins.txt");
    loadCoursesFromFile(courses, teachers, students, "courses.txt");
}

void LMS::run() {
    while (true) {
        cout << "\n========== LMS MAIN MENU ==========" << endl;
        cout << "--- User Creation ---\n"
             << "1) Create Student\n"
             << "2) Create Teacher\n"
             << "3) Create System Admin\n"
             << "4) Create Teaching Assistant\n"
             << "\n--- Course Management ---\n"
             << "5) Create Course\n"
             << "6) Assign Course to Teacher\n"
             << "7) Assign Course to Teaching Assistant\n"
             << "8) Assign Course to System Admin (Supervise)\n"
             << "9) Enroll Student in Course\n"
             << "10) List Courses\n"
             << "11) List Students in Course\n"
             << "12) List Teachers in Course\n"
             << "\n--- User Interaction ---\n"
             << "13) Teacher Announcement\n"
             << "14) Student Comment\n"
             << "\n--- View Dashboards ---\n"
             << "15) View Student Dashboard\n"
             << "16) View Teacher Dashboard\n"
             << "17) View System Admin Dashboard\n"
             << "18) View Teaching Assistant Dashboard\n"
             << "\n--- Admin Functions ---\n"
             << "19) View System Logs\n"
             << "20) Generate System Report\n"
             << "21) View Supervised Courses\n"
             << "\n--- TA Functions ---\n"
             << "22) Assign Task to TA\n"
             << "23) Grade Student (by TA)\n"
             << "24) Conduct Lab Session (by TA)\n"
             << "25) Set TA Performance Rating\n"
             << "26) View TA Assigned Tasks\n"
             << "27) View TA Managed Courses\n"
             << "\n0) Exit\n"
             << "==================================\n"
             << "Choice: ";

        int choice;
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) break;

        // USER CREATION OPTIONS
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
            try {
                auto ta = UserFactory::createTAInteractive();
                if (ta) addTA(move(ta));
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
        
        // COURSE MANAGEMENT OPTIONS
        else if (choice == 5) {
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
        else if (choice == 6) {
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
            try {
                t->addCourse(c);
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
        else if (choice == 7) {
            int taId; 
            string cname;
            cout << "Teaching Assistant ID: "; 
            cin >> taId; 
            cin.ignore();
            cout << "Course name: "; 
            getline(cin, cname);
            
            TeachingAssistant* ta = findTAById(taId);
            Course* c = findCourseByName(cname);
            if (!ta || !c) { 
                cout << "Not found.\n"; 
                continue; 
            }
            try {
                // TA inherits addCourse() from Teacher
                ta->addCourse(c);
                cout << "[SUCCESS] " << ta->getName() << " is now assigned to " 
                     << c->getCourseName() << endl;
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
        else if (choice == 8) {
            int adminId; 
            string cname;
            cout << "System Admin ID: "; 
            cin >> adminId; 
            cin.ignore();
            cout << "Course name: "; 
            getline(cin, cname);
            
            SystemAdmin* admin = findSystemAdminById(adminId);
            Course* c = findCourseByName(cname);
            if (!admin || !c) { 
                cout << "Not found.\n"; 
                continue; 
            }
            try {
                admin->superviseCourse(c);
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
        else if (choice == 9) {
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
            try {
                c->enrollStudent(s);
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
        else if (choice == 10) {
            if (courses.empty()) {
                cout << "\n[INFO] No courses available.\n";
            } else {
                cout << "\n========== Available Courses ==========" << endl;
                for (size_t i = 0; i < courses.size(); ++i) {
                    cout << (i + 1) << ". " << courses[i]->getCourseName() << endl;
                }
                cout << "========================================" << endl;
            }
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
            c->displayStudentList();
        }
        else if (choice == 12) {
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
        
        // USER INTERACTION OPTIONS
        else if (choice == 13) {
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
        else if (choice == 14) {
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
        
        // VIEW DASHBOARD OPTIONS
        else if (choice == 15) {
            int studentId;
            cout << "Enter Student ID: ";
            cin >> studentId;
            cin.ignore();
            
            Student* student = findStudentById(studentId);
            if (!student) {
                cout << "Student not found.\n";
                continue;
            }
            student->displayDashboard();
        }
        else if (choice == 16) {
            int teacherId;
            cout << "Enter Teacher ID: ";
            cin >> teacherId;
            cin.ignore();
            
            Teacher* teacher = findTeacherById(teacherId);
            if (!teacher) {
                cout << "Teacher not found.\n";
                continue;
            }
            teacher->displayDashboard();
        }
        else if (choice == 17) {
            int adminId;
            cout << "Enter System Admin ID: ";
            cin >> adminId;
            cin.ignore();
            
            SystemAdmin* admin = findSystemAdminById(adminId);
            if (!admin) {
                cout << "System Admin not found.\n";
                continue;
            }
            admin->displayDashboard();
        }
        else if (choice == 18) {
            int taId;
            cout << "Enter Teaching Assistant ID: ";
            cin >> taId;
            cin.ignore();
            
            TeachingAssistant* ta = findTAById(taId);
            if (!ta) {
                cout << "Teaching Assistant not found.\n";
                continue;
            }
            ta->displayDashboard();
        }
        
        // ADMIN FUNCTIONS
        else if (choice == 19) {
            int adminId;
            cout << "Enter System Admin ID: ";
            cin >> adminId;
            cin.ignore();
            
            SystemAdmin* admin = findSystemAdminById(adminId);
            if (!admin) {
                cout << "System Admin not found.\n";
                continue;
            }
            admin->viewSystemLogs();
        }
        else if (choice == 20) {
            int adminId;
            cout << "Enter System Admin ID: ";
            cin >> adminId;
            cin.ignore();
            
            SystemAdmin* admin = findSystemAdminById(adminId);
            if (!admin) {
                cout << "System Admin not found.\n";
                continue;
            }
            admin->generateSystemReport();
        }
        else if (choice == 21) {
            int adminId;
            cout << "Enter System Admin ID: ";
            cin >> adminId;
            cin.ignore();
            
            SystemAdmin* admin = findSystemAdminById(adminId);
            if (!admin) {
                cout << "System Admin not found.\n";
                continue;
            }
            admin->viewSupervisedCourses();
        }
        
        // TA FUNCTIONS
        else if (choice == 22) {
            int taId;
            cout << "Enter Teaching Assistant ID: ";
            cin >> taId;
            cin.ignore();
            
            TeachingAssistant* ta = findTAById(taId);
            if (!ta) {
                cout << "Teaching Assistant not found.\n";
                continue;
            }
            
            cout << "Enter task description: ";
            string task;
            getline(cin, task);
            try {
                ta->assignTask(task);
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
        else if (choice == 23) {
            int taId;
            cout << "Enter Teaching Assistant ID: ";
            cin >> taId;
            cin.ignore();
            
            TeachingAssistant* ta = findTAById(taId);
            if (!ta) {
                cout << "Teaching Assistant not found.\n";
                continue;
            }
            
            cout << "Enter student name: ";
            string studentName;
            getline(cin, studentName);
            
            cout << "Enter score (0-100): ";
            double score;
            cin >> score;
            cin.ignore();
            
            cout << "Enter feedback: ";
            string feedback;
            getline(cin, feedback);
            
            try {
                ta->gradeStudent(studentName, score, feedback);
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
        else if (choice == 24) {
            int taId;
            cout << "Enter Teaching Assistant ID: ";
            cin >> taId;
            cin.ignore();
            
            TeachingAssistant* ta = findTAById(taId);
            if (!ta) {
                cout << "Teaching Assistant not found.\n";
                continue;
            }
            
            cout << "Enter course name: ";
            string cname;
            getline(cin, cname);
            Course* c = findCourseByName(cname);
            if (!c) {
                cout << "Course not found.\n";
                continue;
            }
            
            cout << "Enter session details: ";
            string sessionDetails;
            getline(cin, sessionDetails);
            
            try {
                ta->conductLabSession(c, sessionDetails);
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
        else if (choice == 25) {
            int taId;
            cout << "Enter Teaching Assistant ID: ";
            cin >> taId;
            cin.ignore();
            
            TeachingAssistant* ta = findTAById(taId);
            if (!ta) {
                cout << "Teaching Assistant not found.\n";
                continue;
            }
            
            cout << "Enter performance rating (0.0 - 5.0): ";
            float rating;
            cin >> rating;
            cin.ignore();
            
            try {
                ta->setRating(rating);
            } catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
            }
        }
        else if (choice == 26) {
            int taId;
            cout << "Enter Teaching Assistant ID: ";
            cin >> taId;
            cin.ignore();
            
            TeachingAssistant* ta = findTAById(taId);
            if (!ta) {
                cout << "Teaching Assistant not found.\n";
                continue;
            }
            ta->viewAssignedTasks();
        }
        else if (choice == 27) {
            int taId;
            cout << "Enter Teaching Assistant ID: ";
            cin >> taId;
            cin.ignore();
            
            TeachingAssistant* ta = findTAById(taId);
            if (!ta) {
                cout << "Teaching Assistant not found.\n";
                continue;
            }
            
            cout << "\n========== Courses Managed by " << ta->getName() << " ==========" << endl;
            const auto& managedCourses = ta->getManagedCourses();
            if (managedCourses.empty()) {
                cout << "[INFO] Not assigned to any courses.\n" << endl;
            } else {
                for (size_t i = 0; i < managedCourses.size(); ++i) {
                    cout << (i + 1) << ". " << managedCourses[i]->getCourseName() << endl;
                }
            }
            cout << "================================================" << endl;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
}
