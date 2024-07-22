#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Service {
    string serv;
public:
    Service(string s) : serv(s) {}
    Service() {}
    void setService(string service) {
        serv = service;
    }
    ~Service() {}
    
    string getServ() {
        return this->serv;
    }
};

class Room {
protected:
    bool isReserved;
    int roomNumber;
    int numBed;
    Service **services;
    string type;
    double costPerNight;
public:
    Room() : isReserved(false), roomNumber(0), numBed(0), services(nullptr),costPerNight(0.0) {}
    virtual void Reserve() {
        isReserved = true;
    }
    virtual bool Reserved() {
        return isReserved;
    }
    virtual int getRoomNum() {
        return roomNumber;
    }
    Room(int rnum, int nB, string t,double cpn) : roomNumber(rnum), numBed(nB), isReserved(false), services(nullptr), type(t), costPerNight(cpn) {}
    virtual ~Room() {
        if (services) {
            for (int i = 0; i < numBed; i++) {
                delete services[i];
            }
            delete[] services;
        }
    }
    virtual string getType() {
        return type;
    }
    virtual void printServices() = 0;
    virtual double getCPN()
    {
    	return costPerNight;
	}
};

class Standard : public Room {
public:
    Standard(int r) : Room(r, 2, "Standard",10.0) {
        services = new Service*[2];
        services[0] = new Service("Standard Services");
        services[1] = new Service("Wifi");
    }
    ~Standard() {
        for (int i = 0; i < 2; i++) {
            delete services[i];
        }
        delete[] services;
    }
    void printServices() {
        cout << "Services" << endl;
        for (int i = 0; i < 2; i++) {
            cout << i + 1 << " : \t" << services[i]->getServ() << endl;
        }
    }
};

class Executive : public Room {
public:
    Executive(int r) : Room(r, 3, "Executive",25.0) {
        services = new Service*[3];
        services[0] = new Service("Executive Protocol");
        services[1] = new Service("Wifi");
        services[2] = new Service("Breakfast and Dinner Buffet");
    }
    ~Executive() {
        for (int i = 0; i < 3; i++) {
            delete services[i];
        }
        delete[] services;
    }
    void printServices() {
        cout << "Services" << endl;
        for (int i = 0; i < 3; i++) {
            cout << i + 1 << " : \t" << services[i]->getServ() << endl;
        }
    }
};

class VIP : public Room {
public:
    VIP(int r) : Room(r, 3, "VIP",50.0) {
        services = new Service*[3];
        services[0] = new Service("VIP Protocol");
        services[1] = new Service("Wifi");
        services[2] = new Service("Breakfast and Dinner Buffet");
    }
    ~VIP() {
        for (int i = 0; i < 3; i++) {
            delete services[i];
        }
        delete[] services;
    }
    void printServices() {
        cout << "Services" << endl;
        for (int i = 0; i < 3; i++) {
            cout << i + 1 << " : \t" << services[i]->getServ() << endl;
        }
    }
};

class Guest {
public:
    int bookedRooms;
    string loginID;
    string name;
    Room** rooms;
    double Invoice;
    Guest() : bookedRooms(0), rooms(nullptr) {}
    Guest(string naam, string login) : name(naam), loginID(login), bookedRooms(0), rooms(nullptr),Invoice(0.0) {}
    ~Guest() {
        if (rooms != nullptr) {
            for (int i = 0; i < bookedRooms; i++) {
                delete rooms[i];
            }
            delete[] rooms;
        }
    }
    void Book(Room* obj) {
        Room** temp = new Room*[bookedRooms + 1];
        for (int i = 0; i < bookedRooms; i++) {
            temp[i] = rooms[i];
        }
        temp[bookedRooms] = obj;
        delete[] rooms;
        rooms = temp;
        bookedRooms++;
    }
    void Cancel(int rm) {
        int cIndex = -1;
        for (int i = 0; i < bookedRooms; i++) {
            if (rm == rooms[i]->getRoomNum()) {
                cIndex = i;
                break;
            }
        }
        if (cIndex == -1) return; // Room not found

        Room** temp = new Room*[bookedRooms - 1];
        for (int i = 0, j = 0; i < bookedRooms; i++) {
            if (i != cIndex) {
                temp[j++] = rooms[i];
            }
        }
        //delete rooms[cIndex];
        delete[] rooms;
        rooms = temp;
        bookedRooms--;
        system("cls");
        cout << "\n\n   The Room you selected has been successfully cancelled" << endl << endl;
    }
    void DisplayReservations() {
    	system("cls");
        cout << "Total Number of Rooms Reserved: " << bookedRooms << endl;
        for (int i = 0; i < bookedRooms; i++) {
            cout << rooms[i]->getRoomNum() << "  ->  ";
            rooms[i]->printServices();
            cout << endl;
        }
    }
    string getLoginID() {
        return loginID;
    }
    string getName() {
        return name;
    }
    double getInvoice(int nights)
    {
    	Invoice = 0.0;
    	for(int i = 0 ; i < bookedRooms;i++)
    	{
    		Invoice += rooms[i]->getCPN();
		}
		Invoice *= nights;
		return Invoice;
	}
};

Guest** guests = nullptr;
int numGuest = 0;
int maxStandardRooms = 10;
int maxExecutiveRooms = 10;
int maxVIPRooms = 10;
int MaxRooms = maxStandardRooms + maxExecutiveRooms + maxVIPRooms;
int nS = 0, nE = 0, nV = 0;

void loadFromFile() {
    ifstream infile("hotel_data.txt");
    if (!infile) {
        //cout << "Error opening file for reading." << endl;
        return;
    }

    string line;
    Guest* newGuest = nullptr;

    while (getline(infile, line)) {
        if (line == "Guest") {
            if (newGuest) {
                Guest** tempGuests = new Guest*[numGuest + 1];
                for (int i = 0; i < numGuest; i++) {
                    tempGuests[i] = guests[i];
                }
                tempGuests[numGuest] = newGuest;
                delete[] guests;
                guests = tempGuests;
                numGuest++;
            }
            newGuest = new Guest();
            getline(infile, line);
            newGuest->name = line;
            getline(infile, line);
            newGuest->loginID = line;
        } else if (line == "Room" && newGuest) {
            string roomType;
            getline(infile, roomType);
            getline(infile, line);
            int roomNum = stoi(line);

            Room* room = nullptr;
            if (roomType == "Standard") {
                room = new Standard(roomNum);
            } else if (roomType == "Executive") {
                room = new Executive(roomNum);
            } else if (roomType == "VIP") {
                room = new VIP(roomNum);
            }

            if (room) {
                newGuest->Book(room);
                if (roomType == "Standard") nS++;
                else if (roomType == "Executive") nE++;
                else if (roomType == "VIP") nV++;
            }
        }
    }

    if (newGuest) {
        Guest** tempGuests = new Guest*[numGuest + 1];
        for (int i = 0; i < numGuest; i++) {
            tempGuests[i] = guests[i];
        }
        tempGuests[numGuest] = newGuest;
        delete[] guests;
        guests = tempGuests;
        numGuest++;
    }

    infile.close();
}

void saveToFile() {
    ofstream outfile("hotel_data.txt");
    if (!outfile) {
        cout << "Error opening file for writing." << endl;
        return;
    }

    for (int i = 0; i < numGuest; i++) {
        outfile << "Guest" << endl;
        outfile << guests[i]->getName() << endl;
        outfile << guests[i]->getLoginID() << endl;
        for (int j = 0; j < guests[i]->bookedRooms; j++) {
            outfile << "Room" << endl;
            outfile << guests[i]->rooms[j]->getType() << endl;
            outfile << guests[i]->rooms[j]->getRoomNum() << endl;
        }
    }

    outfile.close();
}

void InitializeHotel() {
    loadFromFile();
}

void Exit() {
	system("cls");
	cout << " Exiting........" << endl << endl;
    saveToFile();
    for (int i = 0; i < numGuest; i++) 
	{
        //delete guests[i];
    }
    delete[] guests;
}

void AddGuest() {
	system("cls");
    string log;
    string naam;
    cout << "   Enter the Guest's name: ";
    cin.ignore();
    getline(cin, naam);
    system("cls");
    cout << "   Enter the Login ID: ";
    getline(cin, log);
    Guest** temp = new Guest*[numGuest + 1];
    for (int i = 0; i < numGuest; i++) {
        temp[i] = guests[i];
    }
    temp[numGuest] = new Guest(naam, log);
    delete[] guests;
    guests = temp;
    numGuest++;
}

void Checkout() {
	system("cls");
    string log;
    string name;
	int totalNights = 0;
    cout << "   Enter the Login ID to Checkout: ";
    cin.ignore();
    getline(cin, log);
    int index = -1;
    for (int i = 0; i < numGuest; i++) {
        if (guests[i]->getLoginID() == log) {
            index = i;
            break;
        }
    }
    if (index == -1) {
    	system("cls");
        cout << "   No such Login ID exists in the Database" << endl << endl << endl;
        return;
    }
    system("cls");
    cout << "\n\tHow many nights was your stay? : ";
    cin >> totalNights;
    system("cls");
    cout << "\n\n    -------------------- INVOICE -------------------- \n";
    cout << "\n\tYour Net Total is: " << guests[index]->getInvoice(totalNights) <<"$" << endl
	<< "\n\tPress any key to pay" << endl << endl << endl;
	system("pause");
	system("cls");
	cout << "\n\tSuccessfully checked out, Have a nice day!"<<endl;
    Guest** temp = new Guest*[numGuest - 1];
    for (int i = 0, j = 0; i < numGuest; i++) {
        if (i != index) {
            temp[j++] = guests[i];
        }
    }
    //delete guests[index];
    delete[] guests;
    guests = temp;
    numGuest--;
}

void printAll() 
{
	system("cls");
	cout << "    ------------------ Guests List ------------------ " << endl << endl;
    if (numGuest == 0) 
	{
		system("cls");
        cout << "   No Guests to Show." << endl << endl << endl;
        return;
    }
	cout << "   #\t Name\t\t LoginID" << endl << endl;
    for (int i = 0; i < numGuest; i++) 
	{
        cout <<"   " <<i + 1 << "\t " << guests[i]->getName() << "\t   " << guests[i]->getLoginID() << endl << endl;
    }
}

void Access(int logged) 
{
	system("cls");
	cout << endl << endl <<  "   Succesfully Logged in as " << guests[logged]->getName() << endl << endl;
	system("pause");
    cout << endl << endl;
    int choice;
    do {
    	system("cls");
    	cout << "   ------------------- Home Page -------------------" << endl << endl << endl;
        cout << "   Enter your choice" << endl << endl << "   1. Book a room" << endl<< endl << "   2. Cancel Reservation" << endl<< endl << "   3. Show Reservations with Services" << endl<< endl << "   4. Log out" << endl << endl;
        cin >> choice;
        switch (choice) {
            case 1: {
            	system("cls");
                cout << "   Select a plan" << endl<< endl << "   1. Standard" << endl << endl << "   2. Executive" << endl <<"\n   3. VIP " << endl<< "\n   any other input to cancel" << endl;
                int plan = 0;
                cin >> plan;
                if (plan == 1 && nS < maxStandardRooms) {
                    Room* reserve = new Standard(nS + 1);
                    guests[logged]->Book(reserve);
                    nS++;
                } else if (plan == 2 && nE < maxStandardRooms + maxExecutiveRooms) {
                    Room* reserve = new Executive(nE + 1);
                    guests[logged]->Book(reserve);
                    nE++;
                } else if (plan == 3 && nV < MaxRooms) {
                    Room* reserve = new VIP(nV + 1);
                    guests[logged]->Book(reserve);
                    nV++;
                } else {
                	system("cls");
                    cout << "Invalid room plan or rooms full" << endl;
                }
                system("pause");
                cout << endl << endl;
                break;
            }
            case 2: {
            	system("cls");
                int can = 0;
                cout << "Enter the room number you want to cancel" << endl;
                cin >> can;
                guests[logged]->Cancel(can);
                system("pause");
                cout << endl << endl;
                break;
            }
            case 3: {
                guests[logged]->DisplayReservations();
                system("pause");
                cout << endl << endl;
				break;
            }
            case 4: {
                break;
            }
            default: {
            	system("cls");
                cout << "Invalid Choice" << endl;
                break;
            }
        }
    } while (choice != 4);
}

void Login() {
    int logged = -1;
    string log;
    system("cls");
    cout << "   ---------------- Check-In Page ----------------" << endl << endl ;
    cout << "   Enter your LoginID:\t ";
    cin.ignore();
    getline(cin, log);
    for (int i = 0; i < numGuest; i++) {
        if (guests[i]->getLoginID() == log) {
            logged = i;
            break;
        }
    }
    if (logged == -1) 
	{
        cout << "   No such Login ID exist in the DataBase" << endl << endl << endl;
    } 
	else 
	{
        Access(logged);
    }
}

int main() {
    InitializeHotel();
    int choice = 0;
    do {
    	system("cls");
        cout << "   ---------------- Hotel Management System ----------------" << endl << endl << endl
             << "   Enter your Choice" << endl << endl << "   1. Show All Guests " << endl<< endl  << "   2. Add a Guest" << endl<< endl 
             << "   3. Login to Guest ID" << endl<< endl  << "   4. Checkout a Guest" << endl<< endl  << "   5. Exit" << endl << endl << endl
			 << "   ---------------------------------------------------------" << endl << endl << endl;
        cin >> choice;
        switch (choice) 
		{
            case 1: 
			{
                printAll();
                system("pause");
                cout << endl << endl;
                break;
            }
            case 2: 
			{
                AddGuest();
                system("pause");
                cout << endl << endl;
				break;
            }
            case 3: 
			{
                Login();
                system("pause");
                cout << endl << endl;
				break;
            }
            case 4: 
			{
                Checkout();
                system("pause");
                cout << endl << endl;
				break;
            }
            case 5: 
			{
                Exit();
                system("pause");		
                cout << endl << endl;
				break;
            }
            default: 
			{
                break;
            }
        }
    } while (choice != 5);

    return 0;
}