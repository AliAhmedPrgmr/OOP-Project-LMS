#include <iostream>
#include <string>
#include <vector>
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
	
	int getID(){return id;}
	
	
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

int main() {
    // 1) Create teacher(s)
    Teacher t1("Ali", 986, "Maths");
    Teacher t2("Sara", 777, "Mathematics");

    // 2) Create students
    Student s1("Ahmed", 326, 3.0);
    Student s2("Hassan", 400, 3.7);
    Student s3("Ayesha", 401, 3.9);

    // 3) Create courses
    Course c1("MATHS");
    Course c2("Calculus");

    // 4) Teachers create/manage courses (this also adds them as observers in the course)
    t1.addCourse(&c1);
    t1.addCourse(&c2);

    t2.addCourse(&c1); // second teacher also “creates/joins” same course in your current logic

    // 5) Show teacher dashboards
    t1.displayDashboard();
    t2.displayDashboard();

    // 6) Enroll students (this adds them as observers too)
    c1 += &s1;
    c1 += &s2;

    c2 += &s3;

    // 7) Display course lists
    cout << "\n--- Students in " << c1.getCourseName() << " ---\n";
    c1.displayStudentList();

    cout << "\n--- Teachers in " << c1.getCourseName() << " ---\n";
    c1.displayTeacherList();

    cout << "\n--- Students in " << c2.getCourseName() << " ---\n";
    c2.displayStudentList();

    cout << "\n--- Teachers in " << c2.getCourseName() << " ---\n";
    c2.displayTeacherList();

    // 8) Teacher announces in c1
    cout << "\n===== Teacher Announcement Test =====\n";
    t1.announce(&c1, "Quiz on Friday. Bring your notebook.");

    // 9) Student comment in c1
    cout << "\n===== Student Comment Test =====\n";
    s1.comment(&c1, "Sir please make it on Monday :(");

    // 10) Teacher announces in c2
    cout << "\n===== Another Course Announcement Test =====\n";
    t1.announce(&c2, "Assignment 1 is uploaded.");

    // 11) Student comments in c2 (only s3 enrolled, plus t1 observer in c2)
    cout << "\n===== Student Comment in c2 =====\n";
    s3.comment(&c2, "Okay sir.");

    // 12) Show student dashboards too
    cout << "\n===== Student Dashboards =====\n";
    s1.displayDashboard();
    s2.displayDashboard();
    s3.displayDashboard();

    return 0;
}
