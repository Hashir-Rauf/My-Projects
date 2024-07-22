#include <iostream>
#include <conio.h>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;
class Course
{
private:
	string name;
	string ID;
	string grade;
	int credithours;
public:
	Course();
	Course(string name, string id, int cH);
	~Course();
	void printInfo();
	void printGrade();
	void setGrade(string g);
	string getGrade();
	string getName();
	string getID();
	int getHour();
	bool operator== (const Course& obj)
	{
		return (name == obj.name && ID == obj.ID);
	}
};
class Account {
private:
	double tuitionFee;
	double scholarship;
	double paidAmount;

public:
	Account();
	Account(double tuitionFee, double scholarship, double paidAmount);
	~Account();
	double calculateNetFee();
	string feeStatus();
	double feePerSemester();
	double paidamount()
	{
		return paidAmount;
	}
	double Scholarship()
	{
		return scholarship;
	}
	double tuit()
	{
		return tuitionFee;
	}
};
class Transcript {
private:
    double gpa[5];
    double totalCGPA;
    double numericGrade[5];
    string courseNames[5];
    string courseIDs[5];
    int creditHours[5];
    int courseCount;
public:
    Transcript();
    ~Transcript();
    void calculateCGPA();
    void printTranscript();
    void setCourseGrade(int index, string grade);
    void setCourseInfo(int index, string name, string id, int hours);
    int CourseCount()
    {
    	return courseCount;
	}
	void courseCountinc()
	{
		courseCount++;
	}
};


class Student
{
private:
	string name;
	string rollno;
	string degree;
	string section;
	char status;
	string batch;
	Course* courses[5];
	int CourseCount = 0;
	Account account;
	Transcript transcript;
public:
	Student()
	{
		status = 'a';
		for (int i = 0; i < 5; ++i)
		{
			courses[i] = NULL;
		}
	}
	Student(const string& Name, const string& rollno, const string& degree, const string& section, char status, const string& batch, double tuitionFee, double scholarship, double paidAmount) : account(tuitionFee, scholarship, paidAmount)
	{
		status = 'a';
		for (int i = 0; i < 5; ++i)
		{
			courses[i] = NULL;
		}
		this->name = Name;
		this->rollno = rollno;
		this->degree = degree;
		this->section = section;
		this->status = status;
		this->batch = batch;
	}
	~Student();
	friend ostream& operator<<(ostream& out, const Student& obj);
	void printInfo();
	void setDegree(const string& degree);
	void setSection(const string& section);
	void setStatus(char status);
	Account getAccount();
	void setAccount(Account acc);
	Course* getcourses() {
		//added by manahal
		return courses[0];
	}
	void setName(const string& Name)
	{
		name = Name;
	}
	void setRoll(const string& rollno)
	{
		this->rollno = rollno;
	}
	void setBatch(const string& batch)
	{
		this->batch = batch;
	}
	string getName()
	{
		return this->name;
	}
	string getRoll()
	{
		return this->rollno;
	}
	string getDegree()
	{
		return this->degree;
	}
	string getSection()
	{
		return this->section;
	}
	char getStatus()
	{
		return this->status;
	}
	string getBatch()
	{
		return this->batch;
	}
	void enrollCourse(Course& obj);
	void withdrawCourse(Course& obj);
	 void setCourseGrade(int index, string grade);
    void printTranscript();
	void setGrade(string id, string g) {
    for (int i = 0; i < CourseCount; ++i) {
        if (courses[i]->getID() == id) {
            courses[i]->setGrade(g);
            transcript.setCourseGrade(i, g);
            break;
        }
    }
}
	void printAllCourses()
	{
		if (CourseCount != 0)
		{
			cout << "Course enrolled by the student (" << this->getRoll() << ")" << endl;
			for (int i = 0; i < CourseCount; i++)
			{
				cout << courses[i]->getName() << " " << courses[i]->getID() << endl;
			}
		}
		else
			cout << "No courses are enrolled by the student: " << this->rollno << endl;
	}

	Student& operator=(const Student& obj)
	{
		this->name = obj.name;
		this->batch = obj.batch;
		this->status = obj.status;
		this->degree = obj.degree;
		this->section = obj.section;
		this->rollno = obj.rollno;
		return *this;
	}
	string getCourse(int i)
	{
		if (CourseCount > 0)
			return courses[i]->getID();
		else
			cout << "No Courses registed" << endl;
	}
	int getCourseCount()
	{
		return CourseCount;
	}
	void getAccountInfo()
	{
		cout << "Tuition Fee: " << account.tuit()<<"$" << endl;
		cout << "Scholarship: " << account.Scholarship() <<"$" << endl;
		cout << "Paid Fee: " << account.paidamount() << "$" << endl;
		cout << "Status: " << account.feeStatus() << endl;
	}
};
void Student::withdrawCourse(Course& obj) {
	bool found = false;
	for (int i = 0; i < CourseCount; ++i) {
		if (courses[i]->getID() == obj.getID()) {
			found = true;
			for (int j = i; j < CourseCount - 1; ++j) {
				courses[j] = courses[j + 1];
			}
			CourseCount--;
			cout << "Course withdrawn successfully." << endl;
			return;
		}

	}
	if (!found) {
		cout << "Course not found in the list of enrolled courses." << endl;
	}
}
void Student::setCourseGrade(int index, string grade) {
    transcript.setCourseGrade(index, grade);
}

void Student::printTranscript() {
	 transcript.calculateCGPA();
    transcript.printTranscript();
}
Account Student::getAccount() {
	return account;
}

void Student::setAccount(Account acc) {
	account = acc;
}

void Student::enrollCourse(Course& obj)
{
	if (CourseCount != 0)
	{

		if (courses[CourseCount - 1]->getName() != obj.getName())
		{
			if (CourseCount < 5)
			{
				
				courses[CourseCount++] = &obj;
				cout << "Course enrolled successfully." << endl;
				transcript.setCourseInfo(transcript.CourseCount(), obj.getName(), obj.getID(), obj.getHour());
    			transcript.courseCountinc();
			}
			else
			{
				cout << "Maximum number of courses reached. Cannot enroll more courses." << endl;
			}
		}
		else
		{
			cout << "Course already enrolled" << endl;
		}
	}
	else
	{

		if (CourseCount < 5)
		{
			courses[CourseCount++] = &obj;
			cout << "Course enrolled successfully." << endl;
		}
		else
		{
			cout << "Maximum number of courses reached. Cannot enroll more courses." << endl;
		}


	}
}
Transcript::Transcript() {
    courseCount = 0;
    totalCGPA = 0.0;
}

Transcript::~Transcript() {}

void Transcript::calculateCGPA() {
    double totalCredits = 0.0;
    double totalGradePoints = 0.0;
    for (int i = 0; i < courseCount; ++i) {
        double gradePoint = 0.0;
        if (numericGrade[i] >= 0) {
            totalCredits += creditHours[i];
            totalGradePoints += numericGrade[i] * creditHours[i];
        }
    }
    if (totalCredits > 0) {
        totalCGPA = totalGradePoints / totalCredits;
    } else {
        totalCGPA = 0.0;
    }
}
void Transcript::printTranscript() {
    cout << "Transcript:" << endl;
    cout << "---------------------" << endl;
    cout << "Course\t\tID\tHours\tGrade" << endl;
    for (int i = 0; i < courseCount; ++i) {
        cout << courseNames[i] << "\t" << courseIDs[i] << "\t" << creditHours[i] << "\t" << numericGrade[i] << endl;
    }
    cout << "---------------------" << endl;
    cout << "Total CGPA: " << totalCGPA << endl;
}

void Transcript::setCourseGrade(int index, string grade) {
    for (int i = 0; i < 5; i++) {
		if (grade == "A+" || grade == "A") {
			numericGrade[i] = 4.0;
		}
		else if (grade == "A-") {
			numericGrade[i] = 3.7;
		}
		else if (grade == "B+") {
			numericGrade[i] = 3.3;
		}
		else if (grade == "B") {
			numericGrade[i] = 3.0;
		}
		else if (grade == "B-") {
			numericGrade[i] = 2.7;
		}
		else if (grade == "C+") {
			numericGrade[i] = 2.3;
		}
		else if (grade == "C") {
			numericGrade[i] = 2.0;
		}
		else if (grade == "C-") {
			numericGrade[i] = 1.7;
		}
		else if (grade == "D+") {
			numericGrade[i] = 1.3;
		}
		else if (grade == "D") {
			numericGrade[i] = 1.0;
		}
		else if (grade == "D-") {
			numericGrade[i] = 0.7;
		}
		else if (grade == "F") {
			numericGrade[i] = 0.0;
		}
		else {
			cout << "INVALID ENTRY.." << endl;
			return;
		}
	}
}

void Transcript::setCourseInfo(int index, string name, string id, int hours) {
    if (index >= 0 && index < 5) {
        courseNames[index] = name;
        courseIDs[index] = id;
        creditHours[index] = hours;
    }
}


Account::Account() {
	tuitionFee = 0.0;
	scholarship = 0.0;
	paidAmount = 0.0;
}

Account::Account(double tuitionFee, double scholarship, double paidAmount) {
	this->tuitionFee = tuitionFee;
	this->scholarship = scholarship;
	this->paidAmount = paidAmount;
}

Account::~Account() {
}

double Account::calculateNetFee() {
	return tuitionFee - scholarship - paidAmount;
}

string Account::feeStatus() {
	double netFee = calculateNetFee();
	if (netFee > 0)
		return "Pending";
	else if (netFee < 0)
		return "Overpaid";
	else
		return "Paid";
}

double Account::feePerSemester() {
	// Assuming 2 semesters per year
	return calculateNetFee() / 2;
}

string Course::getGrade()
{
	return this->grade;
}
string Course::getName()
{
	return this->name;
}
string Course::getID()
{
	return this->ID;
}
Course::Course()
{
	name = "Unavailable";
	ID = "0000";
	grade = "N/A";
	credithours = 0;
}
Course::Course(string name, string id, int cH)
{

	this->name = name;
	this->ID = id;
	grade = "NA";
	credithours = cH;
}
Course::~Course()
{
}
void Course::printInfo()
{
	if (name[0] != '\0')
		cout << "Course Name: " << this->name << endl
		<< "Course Code: " << this->ID << endl;
	else
		cout << "Data unavailable" << endl;
}
int Course::getHour() {
	return credithours;
}
void Course::printGrade()
{
	cout << "Grade of " << this->ID << " : " << this->grade << endl;
}
void Course::setGrade(string g)
{
	this->grade = g;
}
Student::~Student()
{
}
ostream& operator<<(ostream& out, const Student& obj)
{
	out << obj.name << " " << obj.rollno << " " << obj.degree << " " << obj.section;
	if (obj.status == 'a' || obj.status == 'A')
		out << " Active ";
	else if (obj.status == 'f' || obj.status == 'F')
		out << " Frozen ";
	out << obj.batch << endl;
	return out;
}
void Student::printInfo() {
	cout << "Name: " << name << endl;
	cout << "Roll No: " << rollno << endl;
	cout << "Degree: " << degree << endl;
	cout << "Section: " << section << endl;
	cout << "Status: ";
	if (status == 'a' || status == 'A') {
		cout << "Active" << endl;
	}
	else if (status == 'f' || status == 'F') {
		cout << "Frozen" << endl;
	}
	cout << "Batch: " << batch << endl;
}

void Student::setDegree(const string& degree)
{
	this->degree = degree;
}
void Student::setSection(const string& section)
{
	this->section = section;
}

void Student::setStatus(char status) {
	if (status == 'a' || status == 'A' || status == 'F' || status == 'f') {
		this->status = status;
	}
}
int studentCount = 0;
Student* students = NULL;
void loadData()
{
	ifstream FILE,FILE2;
	FILE.open("students.txt");
	FILE2.open("accounts.txt");
	if (FILE.is_open())
	{
		cout << "File loaded successfully" << endl;
		FILE >> studentCount;
		FILE.ignore();
		students = new Student[studentCount];
		for (int i = 0; i < studentCount; i++)
		{
			string name, rollno, degree, sect, bat;
			char stats;

			getline(FILE, name);
			getline(FILE, rollno);
			getline(FILE, degree);
			getline(FILE, sect);
			FILE >> stats;
			FILE.ignore();
			getline(FILE, bat);
			double tuitionFee, scholarship, paidAmount;
			FILE2 >> tuitionFee;
			FILE2 >> scholarship;
			FILE2 >> paidAmount;

			students[i] = Student(name, rollno, degree, sect, stats, bat, tuitionFee, scholarship, paidAmount);
		}
		FILE.close();
	}
	else
	{
		cout << "Failed to load the file" << endl;
	}
}
void addStudent() {
	Student* temp = new Student[studentCount + 1];
	for (int i = 0; i < studentCount; i++) {
		temp[i] = students[i];
	}
	delete[] students;
	string name, rollno, degree, sect, bat;
	char stats;
	cin.ignore();
	cout << "Enter Name of the student:";
	getline(cin, name);
	cout << "Enter rollno of " << name << " : ";
	getline(cin, rollno);
	cout << "Enter degree: ";
	getline(cin, degree);
	cout << "Enter Section: ";
	getline(cin, sect);
	cout << "Enter Status: (A)ctive. (F)rozen. : ";
	do {
		stats = _getch();
	} while (stats != 'a' && stats != 'f' && stats != 'A' && stats != 'F');
	cout << "\nEnter Batch: ";
	getline(cin, bat);
	double tuitionFee, scholarship, paidAmount;
	cout << "Enter tuition fee: ";
	cin >> tuitionFee;
	cout << "Enter scholarship: ";
	cin >> scholarship;
	cout << "Enter paid amount: ";
	cin >> paidAmount;

	studentCount++;
	temp[studentCount - 1] = Student(name, rollno, degree, sect, stats, bat, tuitionFee, scholarship, paidAmount);
	students = new Student[studentCount];
	for (int i = 0; i < studentCount; i++) {
		students[i] = temp[i];
	}
	delete[] temp;
}
void printStudents()
{

	cout << "STUDENT #     NAME\t ROLL\tDEGREE\tSECTION\tSTATUS  BATCH" << endl;
	for (int i = 0; i < studentCount; i++) {
		cout << "Student #" << i + 1 << " : " << students[i] <<  endl;
	}
}

void exit()
{
	ofstream FILE("students.txt"),FILE2("accounts.txt");
	if (FILE.is_open())
	{
		FILE << studentCount << endl;
		for (int i = 0; i < studentCount; i++)
		{
			FILE << students[i].getName() << endl;
			FILE << students[i].getRoll() << endl;
			FILE << students[i].getDegree() << endl;
			FILE << students[i].getSection() << endl;
			FILE << students[i].getStatus() << endl;
			FILE << students[i].getBatch() << endl;
			Account acc = students[i].getAccount();
			FILE2 << acc.calculateNetFee() << endl;
			FILE2 << acc.feeStatus() << endl;
			FILE2 << acc.feePerSemester() << endl;
		}
		cout << "File saved succesfully" << endl;

	}
	else
	{
		cout << "Error saving the data" << endl;
	}
}
int login()
{
	int loginIndex = -1;
	string log;
	cout << "Enter the Roll No to login" << endl;
	cin.ignore();
	getline(cin, log);
	for (int i = 0; i < studentCount; i++)
	{
		string temp;
		temp = students[i].getRoll();
		if (log == temp)
		{
			loginIndex = i;
		}
		else
		{
			continue;
		}
	}
	if (loginIndex == -1)
	{
		cout << "Roll no Not found" << endl << endl;
	}
	if (loginIndex >= 0)
		return loginIndex;
}
void removeStudent() {
    string rollNumber;
    cout << "Enter the roll number of the student to remove: ";
    cin >> rollNumber;
    
    int index = -1;
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].getRoll() == rollNumber) {
            index = i;
            break;
        }
    }
    
    if (index != -1) {
        // Shift all elements after the found index to the left
        for (int i = index; i < studentCount - 1; ++i) {
            students[i] = students[i + 1];
        }
        studentCount--;
        cout << "Student with roll number " << rollNumber << " removed successfully." << endl;
    } else {
        cout << "Student with roll number " << rollNumber << " not found." << endl;
    }
}
int main()
{
	int index = -1;
	Course MVC("Multivariable Calculus", "MT1008",3), OOP("Object Oriented Programming", "CS1004",3), EW("Expository Writing", "SS1008",2), DE("Differential Equations", "MT1004",3), DLD("Digital Logic Design", "ET2004",3), IS("Islamic Studies", "SS2006",3), PS("Pakistan Studies", "SS2007",3);
	int choice = 0;

	do
	{
			cout << endl << endl <<"                      Student Management System Home Page                           " << endl;
		cout << "------------------------------------------------------------------------------------" << endl 
		     << "                                                                                     |" << endl;
		cout << "Enter your choice                                                                    |" << endl;
		cout << "1. Load Students Data                                                                |" << endl
			<< "2. Print Students Information                                                        |" << endl
			<< "3. Add a new student                                                                 |" << endl
			<< "4. Remove a student                                                                  |" << endl
			<< "5. Login                                                                             |" << endl
			<< "6. Exit                                                                              |" << endl;
			cout << "------------------------------------------------------------------------------------" << endl << endl;
		cin >> choice;
		switch (choice)
		{
		case 1:
		{
			loadData();
			break;
		}
		case 2:
		{
			printStudents();
			break;
		}
		case 4:
			{
				removeStudent();
				break;
			}
		case 6:
		{

			exit();
			break;
		}
		case 3:
		{
			addStudent();
			break;
		}
		case 5:
		{
			index = login();
			if (index != -1)
			{

				cout << endl << endl << "\aLogged in as " << students[index].getName() << endl << endl;
				int Choice = 0;
				do
				{
					cout <<endl << endl <<students[index].getName() <<"\t\t" << students[index].getRoll() << endl;
					cout << "-----------------------------------------------" << endl << endl;
					cout << "Enter your Choice" << endl
						<< "1. Show Information" << endl
						<< "2. Show Enrolled Courses" << endl
						<< "3. Enroll Courses" << endl
						<< "4. Withdraw Courses" << endl
						<< "5. Change Status" << endl
						<< "6. Change Section" << endl
						<< "7. Access Accounts" << endl
						<< "8. Show Transcript" << endl
						<< "9. Set Grade" << endl
						<< "10. Go back to login Page" << endl  << endl;
					cout <<"------------------------------------------------" << endl << endl;
					cin >> Choice;
					switch (Choice)
					{
					case 1:
					{
						students[index].printInfo();
						break;
					}
					case 2:
					{
						students[index].printAllCourses();
						break;
					}
					case 3:
					{
						int choice = 0;
						cout << "List of available courses:" << endl
							<< "1. " << MVC.getName() <<"\t" << MVC.getID() << endl
							<< "2. " <<DE.getName() << "\t" <<DE.getID() << endl
							<< "3. " << OOP.getName() << "\t" << OOP.getID() << endl
							<< "4. " << EW.getName() << "\t" << EW.getID() << endl
							<< "5. " << IS.getName() << "\t" << IS.getID() << endl
							<< "6. " << DLD.getName() << "\t" << DLD.getID() << endl
							<< "7. " << PS.getName() << "\t" << PS.getID() << endl
							<< "8. Cancel" << endl
							<< "Enter the number of the course you want to enroll:";
						cin >> choice;
						switch (choice)
						{
						case 1:
						{
							students[index].enrollCourse(MVC);
							break;
						}
						case 2:
						{
							students[index].enrollCourse(DE);
							break;
						}
						case 3:
						{
							students[index].enrollCourse(OOP);
							break;
						}
						case 4:
						{
							students[index].enrollCourse(EW);
							break;
						}
						case 5:
						{
							students[index].enrollCourse(IS);
							break;
						}
						case 6:
						{
							students[index].enrollCourse(DLD);
							break;
						}
						case 7:
						{
							students[index].enrollCourse(PS);
							break;
						}
						default:
						{
							break;
						}
						}
						break;
					}
					case 4: {
						string courseID;
						cout << "Enter the course ID to withdraw: ";
						cin.ignore();
						getline(cin, courseID);
						for (int i = 0; i < students[index].getCourseCount(); i++) {
							if (students[index].getCourse(i) == courseID) {
								Course tempCourse("", courseID,3);
								students[index].withdrawCourse(tempCourse);
								break;
							}
						}
						break;
					}
					case 5: {
						char newStatus;
						cout << "Enter new status (a for Active, f for Frozen): ";
						newStatus = _getche();
						students[index].setStatus(newStatus);
						break;
					}
					case 6: {
						string section;
						cout << "Enter new section: ";
						cin.ignore();
						getline(cin, section);
						students[index].setSection(section);
						break;
					}
					case 7:
						{
							if (index != -1) 
							{
      							students[index].getAccountInfo();
  							}
  							break;
						}
					case 8:
						{
							students[index].printTranscript();
							break;
						}
					case 9:
					{
						string grade, course;
						cin.ignore();
						cout << "Enter the Course Code" << endl;
						getline(cin,course);
						cout << "Enter Grade" << endl;
						getline(cin,grade);
						students[index].setGrade(course,grade);
						break;
					}
					case 10:
						{
							break;
						}
					default:
						break;
					}

				} while (Choice != 10);
			}
			break;
		}
		default:
		{
			break;
		}
		}
	} while (choice != 6);
	return 0;
}