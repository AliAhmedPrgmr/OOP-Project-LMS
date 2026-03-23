#include <iostream>
#include <string>
#include <vector>
using namespace std;


class IObserver {
public:
    virtual void update(string message) = 0; // Receives notifications
    virtual ~IObserver() {}
};

class User : public IObserver{
protected:
	string name;
	const int id;
	static int totalUsers;
public:
	User(string n,int i):id(i){
		name = n;
		totalUsers++;
	}
	void update(string message) override {
        cout << "Notification to " << name << " (ID: " << id << "): " << message << endl;
    }
	virtual void displayDashboard() = 0;
	static int getTotalUsers(){ return totalUsers; }
	virtual ~User() {}
};
int User::totalUsers = 0;

class Student: public User{
	float cgpa;
public:
	Student(string n,int i,float c):User(n,i){
		cgpa = c;
		cout<<"You are successfully enrolled as a student"<<endl;
	}
	
	void displayDashboard() override {
		cout << "==== Student Portal ====" << endl;
		cout<<"Name: "<<name<<"\nID: "<<id<<"\nCGPA: "<<cgpa<<endl;
	}
	
	void setCGPA(float n){
		if(n >= 0.0 && n <= 4.0)
		cgpa = n;
	}
	string getName(){return name;}
	
};

class Course{
	string courseName;
	vector <Student*> studentList;
public:
	Course(string n):courseName(n){	}
	
	void operator +=(Student* s){
		studentList.push_back(s);
		cout<<s->getName()<<" added to "<<courseName<<endl;
	}
	
	string getCourseName(){ return courseName;	}
	void displayStudentList(){
		for (Student* i: studentList){
			if(i)
			cout<<i->getName()<<endl;
		}
	}
	
	void postAnnouncement(string msg){
		cout<<"New update in "<<courseName<<endl;
		for (Student* s: studentList){
			s->update(msg);
		}
	}
};

class Teacher: public User{
	string subject;
	vector<Course*> managedCourses;
public:
	Teacher(string n,int i,string s):User(n,i){
		subject = s;
		cout<<"You are successfully registered as a teacher"<<endl;
	}
	
	void addCourse(Course* c){
		managedCourses.push_back(c);
	}
	
	vector<Course*> getCourses() {
        return managedCourses;
    }
	
	void displayDashboard() override {
		cout << "==== Teacher Portal ====" << endl;
        cout << "Teacher: " << name << " | Subject: " << subject << endl;
        cout << "Currently managing " << managedCourses.size() << " class(es)." << endl;
        for (Course* i : managedCourses){
        	if(i)
        	cout<<i->getCourseName()<<endl;
		}
		cout << "=========================" << endl;
	}
	
	void announce(Course* c, string message){
		c->postAnnouncement(message);
	}
	
	void setSubject(string s){
		subject = s;
	}
};



class UserFactory {
	static 
};

int main(){
	Teacher t1("Ali",986,"Maths");
	Student s1("ahmed",326,3);
	Course c1("MATHS");
	
	t1.addCourse(&c1);
	
	t1.displayDashboard();
	c1 += &s1;
	
    
}
