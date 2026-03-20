#include <iostream>
#include <string>
#include <vector>
using namespace std;

class User{
protected:
	string name;
	const int id;
	static int totalUsers;
public:
	User(string n,int i):id(i){
		name = n;
		totalUsers++;
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
		cout<<"Name: "<<name<<"\nID: "<<id<<"\nCGPA: "<<cgpa<<endl;
	}
	
	void setCGPA(float n){
		if(n>0.0 && n<4.0)
		cgpa = n;
	}
	string getName(){return name;}
};

class Teacher: public User{
	string subject;
public:
	Student(string n,int i,string s):User(n,i){
		subject = s;
		cout<<"You are successfully registered as a teacher"<<endl;
	}
	
	void displayDashboard() override {
		cout<<"Name: "<<name<<"\nID: "<<id<<"\nSubject: "<<subject<<endl;
	}
	 
	void setSubject(string s){
		subject = s;
	}
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
};

