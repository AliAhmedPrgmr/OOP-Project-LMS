#include <iostream>
#include <string>
#include <vector>

#include <memory>
#include <limits>

using namespace std;
//Forward declaration to resolve circular dependancy
class Course;
class Teacher;
class Student;

class IObserver { 
public:			  
    virtual void update(IObserver* sender,string message) = 0; // Receives notifications
    virtual ~IObserver() {}        //Destructor
};

//Abstract base class for Student and Teacher
class User : public IObserver{
protected:
	string name;
	const int id;
	static int totalUsers;
public:
	//Constructor
	User(string n,int i):id(i){
		name = n;
		totalUsers++;
	}
	//Notification Message
	void update(IObserver* sender, string message) override {
        if (this != sender)
		cout << "Notification to " << name << " (ID: " << id << "): " << message <<"\n"<< endl;
    }
    //Making class abstract
	virtual void displayDashboard() = 0;
	
	static int getTotalUsers(){ return totalUsers; }
	
	string getName(){ return name;	}
	
	int getID() const { return id; }
	
	virtual ~User() {} //Destructor
};
//Static initialization
int User::totalUsers = 0;



class Student: public User{
	float cgpa;
public:
	//Constructor
	Student(string n,int i,float c):User(n,i){
		cgpa = c;
		cout<<"You are successfully enrolled as a student"<<"\n"<<endl;
	}
	//Overriding the pure virtual function
	void displayDashboard() override {
		cout << "==== Student Portal ====" << endl;
		cout<<"Name: "<<name<<"\nID: "<<id<<"\nCGPA: "<<cgpa<<"\n"<<endl;
	}
	
	void setCGPA(float n){
		if(n >= 0.0 && n <= 4.0)
		cgpa = n;
	}
	
	string getName(){return name;}
	

	void comment(Course* c,string s,IObserver* sender);
	//Overloading comment function for the ease of commenting
	//student would not need to include its own pointer while
	//using the comment function
	void comment(Course* c,string s){
		comment(c,s,this);
	}	
};


class Teacher: public User{
	string subject;
	vector<Course*> managedCourses;
public:
	Teacher(string n,int i,string s):User(n,i){
		subject = s;
		cout<<"You are successfully registered as a teacher"<<"\n"<<endl;
	}
	//Defining after course
	void addCourse(Course* c);
	
	const vector<Course*>& getCourses() {
        return managedCourses;
    }
	
	void getManagedCourses();
	
	void displayDashboard() override {
		cout << "==== Teacher Portal ====" << endl;
        cout << "Teacher: " << name << " | Subject: " << subject << endl;
        cout << "Currently managing " << managedCourses.size() << " class(es)." << endl;
        getManagedCourses();
		cout << "=========================" <<"\n"<< endl;
	}
	
	
	//AGAINN....Providing ease of access
	void announce(Course* c,string s,IObserver* sender);
	void announce(Course* c,string s){
		announce(c,s,this);
	}
	
	void setSubject(string s){
		subject = s;
	}
};

class Course{
	string courseName;
	vector <Student*> studentList;
	vector <Teacher*> teacherList;
	vector <IObserver*> observers;
public:
	//Constructor
	Course(string n):courseName(n){	
		cout<<"Successfully created "<<n<<" class.\n"<<endl;
	}
	//Overriding += operator
	void operator +=(Student* s){
		observers.push_back(s);
		studentList.push_back(s);
		cout<<s->getName()<<" added to "<<courseName<<"\n"<<endl;
	}
	
	string getCourseName(){ return courseName;	}
	
	void displayStudentList(){
		for (Student* i: studentList){
			if(i) //checking for nullptr
			cout<<i->getName()<<endl;
		}
	}
	
	void addTeacher( Teacher* t ){
		observers.push_back(t);
		teacherList.push_back(t);
		cout<<"Teacher "<<t->getName()<<" joined "<<courseName<<" class\n"<<endl;
	}
	void displayTeacherList(){
		for (Teacher* i: teacherList){
			if(i) //checking for nullptr
			cout<<i->getName()<<endl;
		}
	}
	//Function for the teacher to post Announcements for
	//the whole class,the teacher himself
	void post(IObserver* sender,string msg){
		cout<<"New update in "<<courseName<<endl;
		for (IObserver* o: observers){
			o->update(sender,msg);
		}
		cout<<endl;
	}
};

void Student::comment(Course* c, string s, IObserver* sender){
    cout << name << " commented in " << c->getCourseName() << endl;
    c->post(sender, s);
}

void Teacher::getManagedCourses(){
		for (Course* i : managedCourses){
        	if(i)
        	cout<<i->getCourseName()<<endl;
		}
	}
void Teacher::announce(Course* c,string s,IObserver* sender){
		cout<<name<<" announced in "<<c->getCourseName()<<endl;
		c->post(sender, s);
	}
void Teacher::addCourse(Course* c){
	managedCourses.push_back(c);
	c->addTeacher(this);
}

class UserFactory {
public:
    static std::unique_ptr<Student> createStudentInteractive() {
        string name; int id; float cgpa;
        cout << "Student name: "; cin >> ws; getline(cin, name);
        cout << "Student id: "; cin >> id;
        cout << "Student cgpa: "; cin >> cgpa;
        return std::make_unique<Student>(name, id, cgpa);
    }

    static std::unique_ptr<Teacher> createTeacherInteractive() {
        string name, subject; int id;
        cout << "Teacher name: "; cin >> ws; getline(cin, name);
        cout << "Teacher id: "; cin >> id;
        cout << "Teacher subject: "; cin >> ws; getline(cin, subject);
        return std::make_unique<Teacher>(name, id, subject);
    }
};

class LMS {
    vector<std::unique_ptr<Student>> students;
    vector<std::unique_ptr<Teacher>> teachers;
    vector<std::unique_ptr<Course>> courses;

    Student* findStudentById(int id) {
        for (auto& s : students) if (s->getID() == id) return s.get();
        return nullptr;
    }

    Teacher* findTeacherById(int id) {
        for (auto& t : teachers) if (t->getID() == id) return t.get(); // you need getID() in Teacher
        return nullptr;
    }

    Course* findCourseByName(const string& name) {
        for (auto& c : courses) if (c->getCourseName() == name) return c.get();
        return nullptr;
    }

public:
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
            cin >> choice;

            if (choice == 0) break;

            if (choice == 1) {
                students.push_back(UserFactory::createStudentInteractive());
            }
            else if (choice == 2) {
                teachers.push_back(UserFactory::createTeacherInteractive());
            }
            else if (choice == 3) {
                string cname;
                cout << "Course name: "; cin >> ws; getline(cin, cname);
                courses.push_back(std::make_unique<Course>(cname));
                cout << "Course created.\n";
            }
            else if (choice == 4) {
                int tid; string cname;
                cout << "Teacher id: "; cin >> tid;
                cout << "Course name: "; cin >> ws; getline(cin, cname);

                Teacher* t = findTeacherById(tid);
                Course* c = findCourseByName(cname);

                if (!t || !c) { cout << "Teacher or course not found.\n"; continue; }
                t->addCourse(c);
            }
            else if (choice == 5) {
                int sid; string cname;
                cout << "Student id: "; cin >> sid;
                cout << "Course name: "; cin >> ws; getline(cin, cname);

                Student* s = findStudentById(sid);
                Course* c = findCourseByName(cname);

                if (!s || !c) { cout << "Student or course not found.\n"; continue; }
                (*c) += s;
            }
            else if (choice == 6) {
                int tid; string cname, msg;
                cout << "Teacher id: "; cin >> tid;
                cout << "Course name: "; cin >> ws; getline(cin, cname);
                cout << "Announcement: "; getline(cin, msg);

                Teacher* t = findTeacherById(tid);
                Course* c = findCourseByName(cname);

                if (!t || !c) { cout << "Teacher or course not found.\n"; continue; }
                t->announce(c, msg);
            }
            else if (choice == 7) {
                int sid; string cname, msg;
                cout << "Student id: "; cin >> sid;
                cout << "Course name: "; cin >> ws; getline(cin, cname);
                cout << "Comment: "; getline(cin, msg);

                Student* s = findStudentById(sid);
                Course* c = findCourseByName(cname);

                if (!s || !c) { cout << "Student or course not found.\n"; continue; }
                s->comment(c, msg);
            }
            else if (choice == 8) {
                cout << "\nCourses:\n";
                for (auto& c : courses) cout << "- " << c->getCourseName() << "\n";
            }
            else if (choice == 9) {
                string cname;
                cout << "Course name: "; cin >> ws; getline(cin, cname);
                Course* c = findCourseByName(cname);
                if (!c) { cout << "Course not found.\n"; continue; }
                c->displayStudentList();
            }
            else if (choice == 10) {
                string cname;
                cout << "Course name: "; cin >> ws; getline(cin, cname);
                Course* c = findCourseByName(cname);
                if (!c) { cout << "Course not found.\n"; continue; }
                c->displayTeacherList();
            }
            else {
                cout << "Invalid choice.\n";
            }
        }
    }
};

int main() {
    LMS system;
    system.run();
    return 0;
}
