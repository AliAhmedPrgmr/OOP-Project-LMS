// Improved LMS.cpp
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

// Forward declarations
class Course;
class Teacher;
class Student;

// Observer interface
class IObserver {
public:
    virtual void update(IObserver* sender, const string& message) = 0;
    virtual ~IObserver() {}
};

// Abstract User class
class User : public IObserver {
private:
    static int totalUsers;
    string name;
    const int id;
public:
    User(const string& n, int i) : name(n), id(i) { totalUsers++; }
    // Notification
    void update(IObserver* sender, const string& message) override {
        if (this != sender)
            cout << "Notification to " << name << " (ID: " << id << "): " << message << endl;
    }
    virtual void displayDashboard() const = 0;
    static int getTotalUsers() { return totalUsers; }
    const string& getName() const { return name; }
    int getID() const { return id; }
    virtual ~User() {}
};
int User::totalUsers = 0;

// Student class
class Student : public User {
private:
    float cgpa;
public:
    Student(const string& n, int i, float c) : User(n,i), cgpa(c) {
        cout << "You are successfully enrolled as a student!" << endl;
    }
    void displayDashboard() const override {
        cout << "==== Student Portal ====" << endl;
        cout << "Name: " << getName() << "\nID: " << getID() << "\nCGPA: " << cgpa << endl;
    }
    void setCGPA(float n) { if (n >= 0.0 && n <= 4.0) cgpa = n; }
    float getCGPA() const { return cgpa; }

    void comment(Course* c, const string& msg, IObserver* sender);
    // For ease of use, student does not need to include its own pointer
    void comment(Course* c, const string& msg) { comment(c, msg, this); }
};

// Teacher class
class Teacher : public User {
private:
    string subject;
    vector<Course*> managedCourses;
public:
    Teacher(const string& n, int i, const string& s) : User(n, i), subject(s) {
        cout << "You are successfully registered as a teacher!" << endl;
    }
    void addCourse(Course* c);
    void displayDashboard() const override {
        cout << "==== Teacher Portal ====" << endl;
        cout << "Teacher: " << getName() << " | Subject: " << subject << endl;
        cout << "Currently managing " << managedCourses.size() << " class(es):" << endl;
        for (const auto* course : managedCourses) {
            if (course) cout << "* " << course->getCourseName() << endl;
        }
        cout << "=========================" << endl;
    }
    void announce(Course* c, const string& msg, IObserver* sender);
    void announce(Course* c, const string& msg) { announce(c, msg, this); }
    void setSubject(const string& s) { subject = s; }
    // Needed for displaying in Course
    const string& getSubject() const { return subject; }
};

// Course class
class Course {
private:
    string courseName;
    vector<Student*> studentList;
    vector<Teacher*> teacherList;
    vector<IObserver*> observers;

public:
    Course(const string& n) : courseName(n) {
        cout << "Successfully created " << n << " class." << endl;
    }
    const string& getCourseName() const { return courseName; }

    void enrollStudent(Student* s) {
        if (s) {
            // Prevent duplicate enrollments
            for (auto* st : studentList)
                if (st == s) {
                    cout << s->getName() << " is already enrolled in " << courseName << endl;
                    return;
                }
            observers.push_back(s);
            studentList.push_back(s);
            cout << s->getName() << " added to " << courseName << endl;
        }
    }
    void addTeacher(Teacher* t) {
        if (t) {
            for (auto* tch : teacherList)
                if (tch == t) {
                    cout << t->getName() << " is already teaching " << courseName << endl; return;
                }
            observers.push_back(t);
            teacherList.push_back(t);
            cout << "Teacher " << t->getName() << " joined " << courseName << " class" << endl;
        }
    }
    void displayStudentList() const {
        if (studentList.empty()) { cout << "(No students enrolled)\n"; return; }
        for (const auto* s : studentList) if (s) cout << "- " << s->getName() << endl;
    }
    void displayTeacherList() const {
        if (teacherList.empty()) { cout << "(No teachers assigned)\n"; return; }
        for (const auto* t : teacherList) if (t) cout << "- " << t->getName() << " (" << t->getSubject() << ")" << endl;
    }
    // Notify all observers except sender
    void post(IObserver* sender, const string& msg) {
        cout << "New update in " << courseName << ":\n";
        for (auto* o : observers) if (o) o->update(sender, msg);
        cout << endl;
    }
};

// Definitions
void Student::comment(Course* c, const string& msg, IObserver* sender) {
    if (c) {
        cout << getName() << " commented in " << c->getCourseName() << endl;
        c->post(sender, msg);
    }
}
void Teacher::announce(Course* c, const string& msg, IObserver* sender) {
    if (c) {
        cout << getName() << " announced in " << c->getCourseName() << endl;
        c->post(sender, msg);
    }
}
void Teacher::addCourse(Course* c) {
    if (c) {
        // Prevent duplicate assignments
        for (auto* crs : managedCourses)
            if (crs == c) return;
        managedCourses.push_back(c);
        c->addTeacher(this);
    }
}

// Factory class to create new users safely
class UserFactory {
public:
    static unique_ptr<Student> createStudentInteractive() {
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
    static unique_ptr<Teacher> createTeacherInteractive() {
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
};

class LMS {
    vector<unique_ptr<Student>> students;
    vector<unique_ptr<Teacher>> teachers;
    vector<unique_ptr<Course>> courses;

    Student* findStudentById(int id) {
        for (auto& s : students) if (s->getID() == id) return s.get();
        return nullptr;
    }
    Teacher* findTeacherById(int id) {
        for (auto& t : teachers) if (t->getID() == id) return t.get();
        return nullptr;
    }
    Course* findCourseByName(const string& name) {
        for (auto& c : courses) if (c->getCourseName() == name) return c.get();
        return nullptr;
    }

public:

        void saveAll() {
    saveStudentsToFile(students, "students.txt");
    saveTeachersToFile(teachers, "teachers.txt");
    saveCoursesToFile(courses, "courses.txt");
}
void loadAll() {
     loadStudentsFromFile(students, "students.txt");
     loadTeachersFromFile(teachers, "teachers.txt");
     loadCoursesFromFile(courses, "courses.txt");
}
    void run() {
        while (true) {
            cout << "\n===== LMS MENU =====\n"
                << "1) Create Student\n"
                << "2) Create Teacher\n"
                << "3) Create Course\n"
                << "4) Assign Course to Teacher\n"
                << "5) Enroll Student in Course\n"
                << "6) Teacher Announcement\n"
                << "7) Student Comment\n"
                << "8) List Courses\n"
                << "9) List Students in Course\n"
                << "10) List Teachers in Course\n"
                << "0) Exit\n"
                << "Choose: ";

            int choice;
            if (!(cin >> choice)) {
                cout << "Please enter a valid menu number.\n";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush newline

            if (choice == 0) break;

            if (choice == 1) {
                students.push_back(UserFactory::createStudentInteractive());
            }
            else if (choice == 2) {
                teachers.push_back(UserFactory::createTeacherInteractive());
            }
            else if (choice == 3) {
                string cname;
                cout << "Course name: "; getline(cin, cname);
                if (!cname.empty() && !findCourseByName(cname)) {
                    courses.push_back(make_unique<Course>(cname));
                    cout << "Course created.\n";
                } else {
                    cout << "Invalid or duplicate course name.\n";
                }
            }
            else if (choice == 4) {
                int tid; string cname;
                cout << "Teacher id: "; cin >> tid; cin.ignore();
                cout << "Course name: "; getline(cin, cname);
                Teacher* t = findTeacherById(tid);
                Course* c = findCourseByName(cname);
                if (!t || !c) { cout << "Teacher or course not found.\n"; continue; }
                t->addCourse(c);
            }
            else if (choice == 5) {
                int sid; string cname;
                cout << "Student id: "; cin >> sid; cin.ignore();
                cout << "Course name: "; getline(cin, cname);
                Student* s = findStudentById(sid);
                Course* c = findCourseByName(cname);
                if (!s || !c) { cout << "Student or course not found.\n"; continue; }
                c->enrollStudent(s);
            }
            else if (choice == 6) {
                int tid; string cname, msg;
                cout << "Teacher id: "; cin >> tid; cin.ignore();
                cout << "Course name: "; getline(cin, cname);
                cout << "Announcement: "; getline(cin, msg);
                Teacher* t = findTeacherById(tid);
                Course* c = findCourseByName(cname);
                if (!t || !c) { cout << "Teacher or course not found.\n"; continue; }
                t->announce(c, msg);
            }
            else if (choice == 7) {
                int sid; string cname, msg;
                cout << "Student id: "; cin >> sid; cin.ignore();
                cout << "Course name: "; getline(cin, cname);
                cout << "Comment: "; getline(cin, msg);
                Student* s = findStudentById(sid);
                Course* c = findCourseByName(cname);
                if (!s || !c) { cout << "Student or course not found.\n"; continue; }
                s->comment(c, msg);
            }
            else if (choice == 8) {
                cout << "\nCourses:\n";
                for (const auto& c : courses) cout << "- " << c->getCourseName() << endl;
            }
            else if (choice == 9) {
                string cname;
                cout << "Course name: "; getline(cin, cname);
                Course* c = findCourseByName(cname);
                if (!c) { cout << "Course not found.\n"; continue; }
                c->displayStudentList();
            }
            else if (choice == 10) {
                string cname;
                cout << "Course name: "; getline(cin, cname);
                Course* c = findCourseByName(cname);
                if (!c) { cout << "Course not found.\n"; continue; }
                c->displayTeacherList();
            }
            else {
                cout << "Invalid choice.\n";
            }
        }
    }
 friend void saveStudentsToFile(const vector<unique_ptr<Student>>& students, const string& filename) 
 friend void loadStudentsFromFile(vector<unique_ptr<Student>>& students, const string& filename)
 friend  void saveTeachersToFile(const vector<unique_ptr<Teacher>>& teachers, const string& filename)
 friend void loadTeachersFromFile(vector<unique_ptr<Teacher>>& teachers, const string& filename)
};


void saveStudentsToFile(const vector<unique_ptr<Student>>& students, const string& filename) {
    std::ofstream ofs(filename, std::ios::trunc);
    for (const auto& s : students) {
        ofs << s->getName() << "," << s->getID() << "," << s->getCGPA() << std::endl;
    }
}

void loadStudentsFromFile(vector<unique_ptr<Student>>& students, const string& filename) {
    std::ifstream ifs(filename);
    std::string line;
    while (std::getline(ifs, line)) {
        std::stringstream ss(line);
        std::string name, id_str, cgpa_str;
        std::getline(ss, name, ',');
        std::getline(ss, id_str, ',');
        std::getline(ss, cgpa_str, ',');
        int id = std::stoi(id_str);
        float cgpa = std::stof(cgpa_str);
        students.push_back(std::make_unique<Student>(name, id, cgpa));
    }
}

        void saveTeachersToFile(const vector<unique_ptr<Teacher>>& teachers, const string& filename) {
    std::ofstream ofs(filename, std::ios::trunc);
    if (!ofs) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return;
    }
    for (const auto& t : teachers) {
        ofs << t->getName() << "," << t->getID() << "," << t->getSubject() << std::endl;
    }
}

void loadTeachersFromFile(vector<unique_ptr<Teacher>>& teachers, const string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) return; // Not really an error first run
    std::string line;
    while (std::getline(ifs, line)) {
        std::stringstream ss(line);
        std::string name, id_str, subject;
        std::getline(ss, name, ',');
        std::getline(ss, id_str, ',');
        std::getline(ss, subject); // Read rest of the line as subject (handles spaces/commas safely)
        int id = std::stoi(id_str);
        teachers.push_back(std::make_unique<Teacher>(name, id, subject));
    }
}


int main() {
    LMS system;
    system.loadAll();    // loads all data at program start
    system.run();        // interactive menu
    system.saveAll();    // saves all data before exit
    return 0;
}
