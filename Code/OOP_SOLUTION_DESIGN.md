# OOP-Based Learning Management System - Step-by-Step Solution

## Table of Contents
1. [System Startup](#step-1--system-startup)
2. [Class Hierarchy Design](#step-2--class-hierarchy-design)
3. [Core Operations (Menu Loop)](#step-3--core-operations-menu-loop)
4. [Observer Pattern Flow](#step-4--observer-pattern-flow)
5. [System Shutdown](#step-5--system-shutdown)
6. [Exception Handling Strategy](#exception-handling-strategy)
7. [Data Persistence Strategy](#data-persistence-strategy)
8. [Design Patterns Applied](#design-patterns-applied)

---

## Step 1 — System Startup

### Purpose
Initialize the LMS system, recover state from persistent storage, and prepare for user interaction.

### Pseudocode
```
BEGIN MAIN
    TRY
        LMS_Instance = LMS.getInstance()           // Singleton pattern
        CALL LMS_Instance.loadAll()                // Restore from files
        
        PRINT "=== Learning Management System ==="
        CALL LMS_Instance.run()                    // Start menu loop
        CALL LMS_Instance.saveAll()                // Persist changes
        PRINT "Goodbye!"
        
    CATCH exception e
        PRINT ERROR message
        RETURN error code
    END TRY
END MAIN
```

### Implementation Details

**File: main.cpp**
```cpp
#include "LMS.h"
#include <iostream>

int main() {
    try {
        LMS::getInstance().loadAll();              // Load persistent data
        cout << "=== Learning Management System ===" << endl;
        LMS::getInstance().run();                   // Main menu loop
        LMS::getInstance().saveAll();               // Save all changes
        cout << "Goodbye!" << endl;
    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }
    return 0;
}
```

### Data Recovery Flow
```
LMS::loadAll()
    ├─→ loadStudentsFromFile("students.txt")
    │    └─→ For each line: Parse name, ID, CGPA
    │        └─→ Create Student object with recovered data
    │
    ├─→ loadTeachersFromFile("teachers.txt")
    │    └─→ For each line: Parse name, ID, subject
    │        └─→ Create Teacher object with recovered data
    │
    ├─→ loadSystemAdminsFromFile("admins.txt")
    │    └─→ For each line: Parse name, ID
    │        └─→ Create SystemAdmin object
    │
    └─→ loadCoursesFromFile("courses.txt")
         └─→ For each course: Parse name, teacherIDs, studentIDs
             └─→ Create Course object
             └─→ Re-link Teacher objects by ID lookup
             └─→ Re-link Student objects by ID lookup
             └─→ Re-establish Observer relationships
```

---

## Step 2 — Class Hierarchy Design

### Inheritance Hierarchy

```
IObserver (Interface)
    ↑
    │ implements
    │
User (Abstract Base Class)
    ├─ name (string)
    ├─ id (int)
    └─ virtual displayDashboard() = 0
    
    ├─→ Student
    │    ├─ cgpa (float)
    │    ├─ courses_enrolled (vector<Course*>)
    │    └─ displayDashboard()
    │
    ├─→ Teacher
    │    ├─ subject (string)
    │    ├─ courses_taught (vector<Course*>)
    │    └─ displayDashboard()
    │
    ├─→ SystemAdmin
    │    ├─ permissions (string)
    │    ├─ supervised_courses (vector<Course*>)
    │    └─ displayDashboard()
    │
    └─→ TeachingAssistant (HYBRID - Multiple Inheritance)
         ├─ Inherits from Student (via virtual inheritance)
         ├─ Inherits from Teacher (via virtual inheritance)
         │   └─ Resolves diamond problem with virtual inheritance
         │
         ├─ email (string)
         ├─ assigned_tasks (vector<string>)
         ├─ rating (float)
         │
         ├─ Methods:
         │  ├─ assignTask(task_string)
         │  ├─ viewAssignedTasks()
         │  ├─ gradeStudent(studentName, score, feedback)
         │  ├─ conductLabSession(course, sessionDetails)
         │  └─ setRating(rating), getRating()
         │
         └─ Overrides:
             └─ displayDashboard() [combines both parents' info]
```

### Class Responsibilities

| Class | Responsibility | Key Methods |
|-------|---------------|-------------|
| **IObserver** | Define observer contract | `update(sender, msg)` |
| **User** | Common user attributes & behavior | `getName()`, `getID()`, `displayDashboard()` |
| **Student** | Track enrollment, view courses, receive updates | `enroll(course)`, `viewCourses()`, `update()` |
| **Teacher** | Manage courses, post announcements | `postAnnouncement(course, msg)`, `gradeStudent(...)` |
| **SystemAdmin** | System administration, supervision | `generateReport()`, `viewLogs()` |
| **TeachingAssistant** | Hybrid role - teach and learn | All of Student + Teacher + specific TA methods |
| **Course** | Course container, notification hub | `enrollStudent()`, `addTeacher()`, `post()` |
| **LMS** | System orchestrator & persistence | `run()`, `loadAll()`, `saveAll()`, `add*()` |

### Virtual Inheritance Diagram (TeachingAssistant Resolution)

```
Without virtual inheritance (DIAMOND PROBLEM):
        User
       /    \
   Student  Teacher
       \    /
    TeachingAssistant
    
Result: TeachingAssistant has TWO copies of User data members
        Name conflict on getName(), getID(), displayDashboard()

With virtual inheritance (SOLUTION):
        User (virtual base)
       /    \
Student    Teacher
(virtual   (virtual
inherit)   inherit)
    \    /
  TeachingAssistant
    
Result: TeachingAssistant has ONE shared User instance
        Virtual method resolution table (vptr) manages dispatch
```

---

## Step 3 — Core Operations (Menu Loop)

### Main Menu Structure
```
WHILE user does not exit:
    DISPLAY menu options
    READ user_choice
    
    CASE user_choice:
        === User Creation ===
        1) CREATE Student
        2) CREATE Teacher
        3) CREATE System Admin
        4) CREATE Teaching Assistant
        
        === Course Management ===
        5) CREATE Course
        6) ASSIGN Course to Teacher
        7) ASSIGN Course to Teaching Assistant
        8) ASSIGN Course to System Admin (Supervise)
        9) ENROLL Student in Course
        10) LIST Courses
        11) LIST Students in Course
        12) LIST Teachers in Course
        
        === User Interaction ===
        13) Teacher ANNOUNCE to Course
        14) Student COMMENT to Course
        
        === Dashboards ===
        15) VIEW Student Dashboard
        16) VIEW Teacher Dashboard
        17) VIEW System Admin Dashboard
        18) VIEW Teaching Assistant Dashboard
        
        === Admin Functions ===
        19) VIEW System Logs
        20) GENERATE System Report
        21) VIEW Supervised Courses
        
        === TA Functions ===
        22) ASSIGN Task to TA
        23) GRADE Student (by TA)
        24) CONDUCT Lab Session (by TA)
        
        25) EXIT System
    END CASE
END WHILE
```

### Detailed Operation Pseudocodes

#### Operation 1: Create Student
```
INPUT: name (string), ID (int), CGPA (float)

TRY
    VALIDATE:
        name != empty          → throw InvalidInputException
        CGPA >= 0 AND CGPA <= 4.0 → throw InvalidInputException
    
    CHECK: findStudentById(ID) == null OR throw DuplicateEntityException
    
    CREATE: student_obj = new Student(name, ID, CGPA)
    CALL: LMS.addStudent(student_obj)
    PRINT: "Student created successfully"
    
CATCH DuplicateEntityException
    PRINT: "Error: Student ID already exists"
CATCH InvalidInputException
    PRINT: "Error: Invalid input parameters"
END TRY
```

#### Operation 5: Create Course
```
INPUT: courseName (string)

TRY
    VALIDATE:
        courseName != empty    → throw InvalidInputException
    
    CHECK: findCourseByName(courseName) == null OR throw DuplicateEntityException
    
    CREATE: course_obj = new Course(courseName)
    CALL: LMS.addCourse(course_obj)
    PRINT: "Course created successfully"
    
CATCH DuplicateEntityException
    PRINT: "Error: Course name already exists"
CATCH InvalidInputException
    PRINT: "Error: Invalid course name"
END TRY
```

#### Operation 6: Assign Teacher to Course
```
INPUT: teacherID (int), courseName (string)

TRY
    RETRIEVE: teacher = findTeacherById(teacherID)
    IF teacher == null → throw NotFoundException
    
    RETRIEVE: course = findCourseByName(courseName)
    IF course == null → throw NotFoundException
    
    CALL: course.addTeacher(teacher)              // Adds to course.teacherList
    CALL: course.registerObserver(teacher)        // Adds to course.observers
    PRINT: "Teacher assigned to course"
    
CATCH NotFoundException
    PRINT: "Error: Teacher or Course not found"
CATCH InvalidOperationException
    PRINT: "Error: Assignment failed"
END TRY
```

#### Operation 9: Enroll Student in Course
```
INPUT: studentID (int), courseName (string)

TRY
    RETRIEVE: student = findStudentById(studentID)
    IF student == null → throw NotFoundException
    
    RETRIEVE: course = findCourseByName(courseName)
    IF course == null → throw NotFoundException
    
    CALL: course.enrollStudent(student)           // Adds to course.studentList
    CALL: course.registerObserver(student)        // Adds to course.observers
    PRINT: "Student enrolled in course"
    
CATCH NotFoundException
    PRINT: "Error: Student or Course not found"
CATCH DuplicateEntityException
    PRINT: "Error: Student already enrolled"
END TRY
```

#### Operation 13: Teacher Announces to Course
```
INPUT: teacherID (int), courseName (string), message (string)

TRY
    RETRIEVE: teacher = findTeacherById(teacherID)
    IF teacher == null → throw NotFoundException
    
    RETRIEVE: course = findCourseByName(courseName)
    IF course == null → throw NotFoundException
    
    VALIDATE:
        message != empty       → throw InvalidInputException
    
    CALL: course.post(teacher, message)           // Triggers Observer pattern
    
CATCH NotFoundException
    PRINT: "Error: Teacher or Course not found"
CATCH InvalidInputException
    PRINT: "Error: Empty message"
END TRY
```

---

## Step 4 — Observer Pattern Flow

### Pattern Overview
The **Observer Pattern** enables automatic notification of all course participants when a course event occurs (announcement, comment).

### Class Structure
```
IObserver (Interface)
├─ update(sender, message)      // Pure virtual - all observers must implement

Course (Subject)
├─ observers: vector<IObserver*>    // List of subscribers
├─ registerObserver(observer)       // Add observer
├─ post(sender, message)            // Notify all observers
    └─ For each observer in observers:
        └─ IF observer != sender:
            └─ observer.update(sender, message)

User (Concrete Observer) - implements IObserver
├─ update(sender, message)
    └─ PRINT: "[Course] {sender.name}: {message}"
```

### Notification Flow Diagram
```
Teacher calls:
├─ course.post(this_teacher, "Quiz on Friday")
│
└─→ Course::post() executes:
    ├─ FOR each observer in course.observers:
    │  ├─ IF observer != sender:
    │  │  └─ observer.update(sender_teacher, "Quiz on Friday")
    │  │      │
    │  │      └─→ Observer::update() executes:
    │  │          ├─ CASE observer type:
    │  │          │  ├─ Student: Display announcement on dashboard
    │  │          │  ├─ Teacher: Display announcement on dashboard
    │  │          │  └─ TA: Display to both student and teacher aspect
    │  │
    │  └─ [Other observers in same iteration]
    │
    └─ END FOR loop
```

### Message Flow Example
```
SCENARIO: Teacher posts announcement in "Data Structures" course

Step 1: Teacher Input
├─ Choose: "Post Announcement"
├─ Enter: Teacher ID = 101
├─ Enter: Course = "Data Structures"
├─ Enter: Message = "Midterm exam on March 15"

Step 2: LMS Processing
├─ Find Teacher(101)
├─ Find Course("Data Structures")
├─ Call: course.post(teacher_ptr, "Midterm exam on March 15")

Step 3: Course Notification
├─ course.post(sender=Teacher_101, msg="Midterm exam on March 15")
│  ├─ Iterate: course.observers = [Student_1, Student_2, Teacher_101, TA_3]
│  │
│  ├─ Observer 1: Student_1
│  │  ├─ IS Student_1 == Teacher_101? NO
│  │  └─ Call: Student_1.update(Teacher_101, "Midterm exam on March 15")
│  │     └─ Output: "[Data Structures] Teacher Ravid: Midterm exam on March 15"
│  │
│  ├─ Observer 2: Student_2
│  │  ├─ IS Student_2 == Teacher_101? NO
│  │  └─ Call: Student_2.update(Teacher_101, "Midterm exam on March 15")
│  │     └─ Output: "[Data Structures] Teacher Ravid: Midterm exam on March 15"
│  │
│  ├─ Observer 3: Teacher_101
│  │  ├─ IS Teacher_101 == Teacher_101? YES
│  │  └─ SKIP (Don't send notification to self)
│  │
│  └─ Observer 4: TA_3
│     ├─ IS TA_3 == Teacher_101? NO
│     └─ Call: TA_3.update(Teacher_101, "Midterm exam on March 15")
│        └─ Output: "[Data Structures] Teacher Ravid: Midterm exam on March 15"

Step 4: Result
└─ All course members EXCEPT sender see the announcement
```

### Implementation Code Pattern
```cpp
// Course.cpp - Notification Handler
void Course::post(IObserver* sender, const std::string& msg) {
    if (msg.empty()) {
        throw InvalidInputException("Course post message cannot be empty.");
    }
    
    cout << "\n--- New update in " << courseName << " ---" << endl;
    
    for (auto* observer : observers) {
        if (observer && observer != sender) {  // Skip sender
            observer->update(sender, msg);      // Virtual dispatch
        }
    }
    
    cout << "-----------------------------------" << endl;
}

// User.cpp - Concrete Observer Implementation
void User::update(IObserver* sender, const std::string& message) {
    User* sender_user = dynamic_cast<User*>(sender);
    if (sender_user) {
        cout << "[Course] " << sender_user->getName() << ": " 
             << message << endl;
    }
}
```

---

## Step 5 — System Shutdown

### Purpose
Ensure all in-memory state is persisted to disk for recovery on next startup.

### Pseudocode
```
BEGIN SHUTDOWN
    TRY
        CALL: saveStudentsToFile(students, "students.txt")
        CALL: saveTeachersToFile(teachers, "teachers.txt")
        CALL: saveSystemAdminsToFile(admins, "admins.txt")
        CALL: saveCoursesToFile(courses, "courses.txt")
        
        PRINT: "All data saved successfully"
        
    CATCH InvalidOperationException
        PRINT: "Error: Failed to save data"
        RETURN false
    END TRY
END SHUTDOWN
```

### Persistence Details

#### Students File Format (students.txt)
```
Format: name|ID|CGPA
Example:
Alice|1001|3.8
Bob|1002|3.5
Charlie|1003|3.2
```

#### Teachers File Format (teachers.txt)
```
Format: name|ID|subject
Example:
Dr. Smith|2001|Mathematics
Dr. Johnson|2002|Physics
Prof. Lee|2003|Chemistry
```

#### Admins File Format (admins.txt)
```
Format: name|ID
Example:
Admin1|3001
Admin2|3002
```

#### Courses File Format (courses.txt)
```
Format: courseName|teacherID1,teacherID2,...|studentID1,studentID2,...
Example:
Calculus I|2001|1001,1002,1003
Physics 101|2002|1001,1004
Chemistry|2003|1002,1005
```

### Recovery Mechanism
```
ON LOAD:
    For each line in students.txt:
        ├─ Parse: name, ID, CGPA
        ├─ Validate: CGPA in [0, 4.0]
        ├─ Check: ID unique
        └─ Create: Student object, add to LMS
    
    For each line in courses.txt:
        ├─ Parse: courseName, teacherIDs, studentIDs
        ├─ Create: Course object
        ├─ For each teacherID: Find Teacher, link to Course
        ├─ For each studentID: Find Student, link to Course
        └─ Restore: Observer relationships
```

---

## Exception Handling Strategy

### Exception Hierarchy
```
std::runtime_error (Standard C++ Base)
    │
    └─→ LMSException (Project Base Exception)
        │
        ├─→ InvalidInputException
        │   ├─ Empty name
        │   ├─ CGPA out of range
        │   ├─ Empty message
        │   └─ Negative numbers where invalid
        │
        ├─→ DuplicateEntityException
        │   ├─ Duplicate Student ID
        │   ├─ Duplicate Teacher ID
        │   ├─ Duplicate Course name
        │   └─ Duplicate enrollment
        │
        ├─→ NotFoundException
        │   ├─ Student not found by ID
        │   ├─ Teacher not found by ID
        │   ├─ Course not found by name
        │   └─ Record not in file
        │
        └─→ InvalidOperationException
            ├─ File I/O failure
            ├─ Permission denied
            ├─ Illegal state transition
            └─ Corrupted data during load
```

### Exception Handling Pattern
```cpp
// Template for all operations
try {
    // Validate inputs
    if (input_invalid) {
        throw InvalidInputException("Descriptive error message");
    }
    
    // Check preconditions
    if (condition_not_met) {
        throw NotFoundException("Resource not found");
    }
    
    // Perform operation
    // ...
    
    cout << "Operation successful" << endl;
    
} catch (const InvalidInputException& e) {
    cerr << "Input Error: " << e.what() << endl;
    // Optionally prompt user to retry
    
} catch (const DuplicateEntityException& e) {
    cerr << "Duplicate Error: " << e.what() << endl;
    
} catch (const NotFoundException& e) {
    cerr << "Not Found Error: " << e.what() << endl;
    
} catch (const InvalidOperationException& e) {
    cerr << "Operation Error: " << e.what() << endl;
    
} catch (const std::exception& e) {
    cerr << "Unexpected Error: " << e.what() << endl;
}
```

### Caller Responsibility
```
- Every user input operation is wrapped in try-catch
- Errors are displayed but don't crash the system
- Menu loop continues after exception
- User can retry operation
- Audit trail logged for critical failures (admins)
```

---

## Data Persistence Strategy

### File I/O Architecture
```
LMS (Orchestrator)
├─ saveAll()
│  ├─ saveStudentsToFile(students, "students.txt")
│  ├─ saveTeachersToFile(teachers, "teachers.txt")
│  ├─ saveSystemAdminsToFile(admins, "admins.txt")
│  └─ saveCoursesToFile(courses, "courses.txt")
│
└─ loadAll()
   ├─ loadStudentsFromFile(students, "students.txt")
   ├─ loadTeachersFromFile(teachers, "teachers.txt")
   ├─ loadSystemAdminsFromFile(admins, "admins.txt")
   └─ loadCoursesFromFile(courses, teachers, students, "courses.txt")
       [Note: Requires teacher/student references for linking]
```

### Serialization Format
```
GENERAL PATTERN:
    - Tab/pipe-delimited fields
    - One record per line
    - No headers
    - Example: name | ID | attribute1 | attribute2 | ...

FILE ORDER MATTERS:
    1. Students FIRST
    2. Teachers SECOND
    3. Admins THIRD
    4. Courses LAST (depends on Students & Teachers)
    
REASON: Courses need to link to loaded Student/Teacher objects by ID
```

### Deserialization Process
```
loadCoursesFromFile(courses, teachers, students, filename):
    FOR each line in courses.txt:
        ├─ Parse: courseName, teacherID_list, studentID_list
        ├─ CREATE: course_obj = new Course(courseName)
        ├─ FOR each teacherID in teacherID_list:
        │  ├─ SEARCH: teachers vector for ID match
        │  ├─ RETRIEVE: teacher_ptr
        │  └─ CALL: course.addTeacher(teacher_ptr)
        │
        ├─ FOR each studentID in studentID_list:
        │  ├─ SEARCH: students vector for ID match
        │  ├─ RETRIEVE: student_ptr
        │  └─ CALL: course.enrollStudent(student_ptr)
        │
        └─ ADD: course_obj to courses vector
    END FOR
```

---

## Design Patterns Applied

### 1. Singleton Pattern (LMS)
**Purpose:** Ensure only one instance of LMS exists globally.

**Implementation:**
```cpp
class LMS {
private:
    static LMS* instance;           // Single static instance
    LMS() = default;                 // Private constructor
    
public:
    LMS(const LMS&) = delete;        // Delete copy constructor
    LMS& operator=(const LMS&) = delete;  // Delete assignment
    
    static LMS& getInstance() {
        if (instance == nullptr) {
            instance = new LMS();
        }
        return *instance;
    }
};
```

**Benefit:** 
- Global access point: `LMS::getInstance()`
- Ensures data consistency (only one system state)
- Controls application lifecycle

### 2. Factory Pattern (UserFactory)
**Purpose:** Encapsulate object creation logic with validation.

**Structure:**
```cpp
class UserFactory {
public:
    static std::unique_ptr<Student> createStudent(
        const std::string& name, int id, float cgpa);
    
    static std::unique_ptr<Teacher> createTeacher(
        const std::string& name, int id, const std::string& subject);
    
    // Each factory method:
    // 1. Validates inputs
    // 2. Throws exceptions for invalid data
    // 3. Returns validated object
};
```

**Benefit:**
- Centralized validation logic
- Consistent error handling
- Decouples object creation from client code

### 3. Observer Pattern (Course Notifications)
**Purpose:** Automatically notify all course participants of events.

**Participants:**
- **Subject:** Course (maintains observer list)
- **Observer:** IObserver interface
- **Concrete Observers:** User (Student/Teacher/TA)

**Flow:**
```
Course.post(sender, message)
    ↓
FOR each observer in observers:
    ├─ IF observer != sender
    └─ observer.update(sender, message)
```

**Benefit:**
- Loose coupling between Course and Participants
- Easy to add new observer types
- Automatic propagation of events

### 4. Template Method Pattern (FileIO)
**Purpose:** Define algorithm structure, let subclasses override specific steps.

**Structure:**
```
loadFromFile(filename):
    ├─ OPEN file
    ├─ FOR each line:
    │  ├─ PARSE line          ← Specific to entity type
    │  └─ CREATE object       ← Specific to entity type
    └─ CLOSE file
```

**Variants:**
- `loadStudentsFromFile()`
- `loadTeachersFromFile()`
- `loadSystemAdminsFromFile()`
- `loadCoursesFromFile()`

### 5. Inheritance & Polymorphism (User Hierarchy)
**Purpose:** Create flexible class structures with shared behavior.

**Structure:**
```
User (abstract base)
├─ Student (concrete)
├─ Teacher (concrete)
├─ SystemAdmin (concrete)
└─ TeachingAssistant (multiple inheritance with virtual base)
    ├─ virtual public Student
    └─ virtual public Teacher
```

**Benefit:**
- Code reuse through inheritance
- Virtual methods for polymorphic behavior
- TeachingAssistant combines two roles naturally

### 6. Multiple Inheritance with Virtual Base (TeachingAssistant)
**Purpose:** Combine Student and Teacher roles while resolving diamond problem.

**Problem:**
```
Without virtual: 
    User fields duplicated in TeachingAssistant
    Ambiguous method calls

With virtual:
    class Student : virtual public User { ... }
    class Teacher : virtual public User { ... }
    class TA : public Student, public Teacher { ... }
    Result: Single User instance in TA
```

**Benefit:**
- TA inherits all Student + Teacher capabilities
- No ambiguity through virtual method resolution table

---

## Complete System State Diagram

```
STARTUP
  │
  ├─→ LMS::getInstance()              [Singleton creation]
  │
  ├─→ loadAll()                       [Data recovery]
  │    ├─ Load students
  │    ├─ Load teachers
  │    ├─ Load admins
  │    └─ Load courses with relinking
  │
  ├─→ run()                           [Menu loop]
  │    ├─→ User selection
  │    ├─→ Operation execution
  │    │    ├─ Input validation
  │    │    ├─ Duplicate detection
  │    │    ├─ Relationship building
  │    │    └─ Observer pattern
  │    ├─→ Exception handling
  │    └─→ Loop until exit
  │
  └─→ saveAll()                       [Persistence]
       ├─ Save students
       ├─ Save teachers
       ├─ Save admins
       └─ Save courses
       
SHUTDOWN
```

---

## Testing Strategy

### Unit Tests (Per Class)
```
Student:
├─ Constructor validation (CGPA bounds)
├─ Enrollment in multiple courses
└─ Dashboard display

Teacher:
├─ Course assignment
├─ Announcement posting
└─ Observer notification

Course:
├─ Student enrollment
├─ Teacher assignment
├─ Observer pattern (post/update)
└─ Duplicate prevention

LMS:
├─ Singleton enforcement
├─ CRUD operations
├─ Exception handling
└─ File I/O
```

### Integration Tests
```
Full Workflow:
├─ Create Student + Teacher + Course
├─ Enroll Student in Course
├─ Assign Teacher to Course
├─ Post announcement
├─ Verify observer notification
├─ Save all data
├─ Load and verify recovered state
└─ Repeat cycle
```

### Edge Cases
```
├─ Empty file loading
├─ Corrupted file format
├─ Duplicate ID in file
├─ Missing referenced object
├─ Concurrent observer notification
├─ Multiple course enrollments
├─ TA acting as student and teacher
└─ Large-scale data (1000+ records)
```

---

## Conclusion

This OOP-based solution implements:
1. **Modularity** through clear class responsibilities
2. **Extensibility** through design patterns (Factory, Observer, etc.)
3. **Robustness** through exception handling and validation
4. **Persistence** through file I/O layer
5. **Flexibility** through inheritance and polymorphism

The architecture is scalable and maintainable for future enhancements such as database backend, authentication, or web UI.
