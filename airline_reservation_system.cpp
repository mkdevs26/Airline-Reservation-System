#include<iostream>
#include<fstream>
#include<string>
using namespace std;

struct Flight
{
	int id;
	string origin;
	string destination;
	string date;
	string departTime;
	string arrivalTime;
	int total;
	int booked;
	int economy;
	int business;
	char** seats = nullptr;
	int rows;
	int cols;
};
struct Booking {
	int id;
	string origin;
	string destination;
	string date;
	string departTime;
	string arrivalTime;
	int fare;
	string cls;
	int seatRow;
	int seatCol;
};
struct Passenger
{
	int id;
	string firstName;
	string lastName;
	char gender;
	string email;
	string phone;
	string password;
	Booking* bookings = nullptr;
	int bookingCount = 0;
};


Flight* flight = nullptr;
int flightCount = 0;
int fcapacity = 10;
Passenger* passenger = nullptr;
int passengerCount = 0;
int pcapacity = 10;
string adminPass = "1234", adminUser = "admin";
int userIndex;

void loadFlights();
void savePassenger();
void appendPassenger();
void loadPassenger();
void saveFlights();
void saveBookings();
void loadBookings();

void user_login();
void userMenu();
void available_flights();
void flight_booking();
void displaySeats(Flight& f);
void cancel_booking();
void generate_report();
void admin_login();
void admin();
void changePassword();
void viewRegisteredFlights();
void addFlight();
void removeFlight();
void updatePassengerDetails();
void addPassenger();
void removePassenger();
void checkReportsPassengers();
void checkReportsFlights();
void viewCurrentPassengers();

int main()
{
	loadFlights();
	loadPassenger();
	loadBookings();
	system("color 06");
	cout << "\t\t-:(UET Airline Management System):-\n\n";
	string choice;
back:
	cout << "--------------- Are you ---------------\n";
	cout << "1. Admin\n";
	cout << "2. Passenger\n";
	cout << "3. Exit\n";
	cout << "---------------------------------------" << endl;
	cout << "Enter choice: ";
	cin >> choice;
	if (choice == "1")
	{
		admin_login();
	}
	else if (choice == "2")
	{
		user_login();
	}
	else if (choice == "3")
	{
		cout << "Program End!";
	}
	else
	{
		cout << "Invalid choice!" << endl;
		cout << "---------------------------------------" << endl;
		goto back;
	}

	delete[] flight;
	flight = nullptr;
	delete[] passenger;
	passenger = nullptr;
	return 0;
}

void loadFlights() {
	ifstream file("Flights.txt");
	if (!file)
	{
		cout << "Flights not Found.";
		return;
	}
	if (!flight) {
		flight = new Flight[fcapacity];
	}
	flightCount = 0;
	while (file >> flight[flightCount].id
		>> flight[flightCount].origin
		>> flight[flightCount].destination
		>> flight[flightCount].date
		>> flight[flightCount].departTime
		>> flight[flightCount].arrivalTime
		>> flight[flightCount].total
		>> flight[flightCount].booked
		>> flight[flightCount].economy
		>> flight[flightCount].business
		>> flight[flightCount].rows
		>> flight[flightCount].cols)
	{
		flight[flightCount].seats = new char* [flight[flightCount].rows];
		for (int i = 0; i < flight[flightCount].rows; i++) {
			flight[flightCount].seats[i] = new char[flight[flightCount].cols];
			for (int j = 0; j < flight[flightCount].cols; j++) {
				file >> flight[flightCount].seats[i][j];
			}
		}
		flightCount++;
		if (flightCount == fcapacity) {
			fcapacity += 5;
			Flight* temp = new Flight[fcapacity];
			for (int i = 0; i < flightCount; i++)
				temp[i] = flight[i];

			delete[] flight;
			flight = temp;
		}
	}
	file.close();
}
void savePassenger() {
	ofstream file("data.txt");
	if (!file)
		return;

	for (int i = 0; i < passengerCount; i++) {
		file << passenger[i].id << " "
			<< passenger[i].firstName << " "
			<< passenger[i].lastName << " "
			<< passenger[i].gender << " "
			<< passenger[i].email << " "
			<< passenger[i].phone << " "
			<< passenger[i].password << endl;
	}
	file.close();
}
void appendPassenger() {
	ofstream file("data.txt", ios::app);
	if (!file)
		return;

	int i = passengerCount;
	file << passenger[i].id << " "
		<< passenger[i].firstName << " "
		<< passenger[i].lastName << " "
		<< passenger[i].gender << " "
		<< passenger[i].email << " "
		<< passenger[i].phone << " "
		<< passenger[i].password << endl;
}
void loadPassenger() {
	ifstream file("data.txt");

	if (!passenger) {
		passenger = new Passenger[pcapacity];
	}
	if (!file) {
		cout << "File not Found.";
		return;
	}
	passengerCount = 0;
	while (file >> passenger[passengerCount].id
		>> passenger[passengerCount].firstName
		>> passenger[passengerCount].lastName
		>> passenger[passengerCount].gender
		>> passenger[passengerCount].email
		>> passenger[passengerCount].phone
		>> passenger[passengerCount].password) {

		passengerCount++;

		if (passengerCount == pcapacity) {
			pcapacity += 5;
			Passenger* temp = new Passenger[pcapacity];
			for (int i = 0; i < passengerCount; i++)
				temp[i] = passenger[i];

			delete[] passenger;
			passenger = temp;
		}
	}
	file.close();
}
void saveBookings() {
	ofstream file("Bookings.txt");
	if (!file)
		return;
	for (int i = 0; i < passengerCount; i++) {
		for (int j = 0; j < passenger[i].bookingCount; j++) {
			Booking b = passenger[i].bookings[j];
			file << passenger[i].email << " "
				<< b.id << " "
				<< b.origin << " "
				<< b.destination << " "
				<< b.date << " "
				<< b.departTime << " "
				<< b.arrivalTime << " "
				<< b.fare << " "
				<< b.cls << " "
				<< b.seatRow << " "
				<< b.seatCol << endl;
		}
	}
	file.close();
}

void loadBookings() {
	ifstream file("Bookings.txt");
	if (!file)
		return;

	string email;
	while (file >> email) {

		int id, fare;
		string origin, destination, date, departTime, arrivalTime, cls;
		int seatRow, seatCol;

		file >> id >> origin >> destination >> date >> departTime >> arrivalTime >> fare >> cls >> seatRow >> seatCol;

		int p_index = -1;

		for (int i = 0; i < passengerCount; i++) {
			if (passenger[i].email == email) {
				p_index = i;
				break;
			}
		}

		if (p_index == -1)
			continue;

		Booking b;
		b.id = id;
		b.origin = origin;
		b.destination = destination;
		b.date = date;
		b.departTime = departTime;
		b.arrivalTime = arrivalTime;
		b.fare = fare;
		b.cls = cls;
		b.seatRow = seatRow;
		b.seatCol = seatCol;

		Booking* temp = new Booking[passenger[p_index].bookingCount + 1];

		for (int i = 0; i < passenger[p_index].bookingCount; i++)
			temp[i] = passenger[p_index].bookings[i];

		delete[] passenger[p_index].bookings;

		passenger[p_index].bookings = temp;
		passenger[p_index].bookings[passenger[p_index].bookingCount] = b;
		passenger[p_index].bookingCount++;
	}
	file.close();
}

void user_login() {
	int id;
	string password;
	bool found = false;
	do {
		cout << "\n\tLogin as Passenger\n";
		cout << "Enter ID:";
		cin >> id;
		cout << "Enter Password: ";
		cin >> password;
		for (int i = 0; i < passengerCount; i++) {
			if (id == passenger[i].id && password == passenger[i].password) {
				found = true;
				userIndex = i;
				break;
			}
		}
		if (found) {
			cout << "\nLogin Successful!" << endl;
			cout << "---------------------------------------" << endl;
			userMenu();
		}
		else {
			bool exists = false;
			for (int i = 0; i < passengerCount; i++) {
				if (id == passenger[i].id) {
					exists = true;
					break;
				}
			}
			if (exists) {
				cout << "\nIncorrect password!" << endl;
			}
			else {
				cout << "\nPassenger id not Found. Contact Admin.\n" << endl;
				return;
			}
		}
	} while (!found);
}
void userMenu() {
	string choice;
	bool run = true;
	do {
		cout << "\n\tWelcome " << passenger[userIndex].firstName << "!" << endl;
		cout << "1. View Available Flights" << endl;
		cout << "2. Book a Flight" << endl;
		cout << "3. Cancel Reservation" << endl;
		cout << "4. Generate Booking Reports" << endl;
		cout << "5. Log Out!" << endl;
		cout << "Enter the choice: ";
		cin >> choice;
		if (choice == "1")			 available_flights();
		else if (choice == "2")		 flight_booking();
		else if (choice == "3")		 cancel_booking();
		else if (choice == "4")		 generate_report();
		else if (choice == "5") {
			cout << "Logging out!";
			run = false;
			return;
		}
		else						cout << "Invalid Input!" << endl;
	} while (run);
}
void available_flights() {
	if (flightCount == 0) {
		cout << "No Flights to show!\n" << endl;
		return;
	}
	cout << "\n\tAvailable Flights" << endl;
	for (int i = 0; i < flightCount; i++) {
		int left = flight[i].total - flight[i].booked;
		cout << "\nFlight ID: " << flight[i].id
			<< "\nFrom: " << flight[i].origin
			<< "  To: " << flight[i].destination
			<< "\nDate: " << flight[i].date
			<< "\nDeparture Time: " << flight[i].departTime
			<< "\t\tArrival Time: " << flight[i].arrivalTime
			<< "\nTotal Seats: " << flight[i].total
			<< "\t\tBooked Seats: " << flight[i].booked
			<< "\t\tRemaining Seats: " << left
			<< "\nEconomy Class Fare: " << flight[i].economy << " PKR"
			<< "\tBusiness Class Fare: " << flight[i].business << " PKR"
			<< "\n--------------------------------------------------------------------";
	}
	cout << endl;
}
void flight_booking()
{
	int flightID, flightIndex;
	bool found = false;

	if (flightCount == 0) {
		cout << "No flights available!" << endl;
		return;
	}
	cout << "\nEnter Flight ID to book: ";
	cin >> flightID;
	for (int i = 0; i < flightCount; i++) {
		if (flight[i].id == flightID) {
			flightIndex = i;
			found = true;
			break;
		}
	}
	if (!found) {
		cout << "Invalid Flight ID!" << endl;
		return;
	}
	displaySeats(flight[flightIndex]);
	int row, col;
	cout << "Enter row number (1-" << flight[flightIndex].rows << "): ";
	cin >> row;
	cout << "Enter seat number (1-" << flight[flightIndex].cols << "): ";
	cin >> col;
	row--; col--;
	if (row < 0 || row >= flight[flightIndex].rows || col < 0 || col >= flight[flightIndex].cols) {
		cout << "Invalid seat selection!" << endl;
		return;
	}
	if (flight[flightIndex].seats[row][col] == 'B') {
		cout << "Seat already booked!" << endl;
		return;
	}
	if (flight[flightIndex].booked >= flight[flightIndex].total) {
		cout << "No seats available!" << endl;
		return;
	}

	int choice;
	cout << "\nSelect Class:\n";
	cout << "1. Economy (" << flight[flightIndex].economy << " PKR)\n";
	cout << "2. Business (" << flight[flightIndex].business << " PKR)\n";
	cout << "Enter choice: ";
	cin >> choice;

	if (choice != 1 && choice != 2) {
		cout << "Invalid choice!" << endl;
		return;
	}
	flight[flightIndex].seats[row][col] = 'B';
	flight[flightIndex].booked++;

	cout << "\nBooking Successful!";

	Booking b;
	b.id = flight[flightIndex].id;
	b.origin = flight[flightIndex].origin;
	b.destination = flight[flightIndex].destination;
	b.date = flight[flightIndex].date;
	b.departTime = flight[flightIndex].departTime;
	b.arrivalTime = flight[flightIndex].arrivalTime;
	if (choice == 1) {
		b.fare = flight[flightIndex].economy;
		b.cls = "Economy";
	}
	else if (choice == 2) {
		b.fare = flight[flightIndex].business;
		b.cls = "Business";
	}
	b.seatRow = row + 1;
	b.seatCol = col + 1;

	Booking* temp = new Booking[passenger[userIndex].bookingCount + 1];
	for (int i = 0; i < passenger[userIndex].bookingCount; i++)
		temp[i] = passenger[userIndex].bookings[i];
	delete[] passenger[userIndex].bookings;

	passenger[userIndex].bookings = temp;
	passenger[userIndex].bookings[passenger[userIndex].bookingCount] = b;
	passenger[userIndex].bookingCount++;
	saveBookings();
	saveFlights();
}
void displaySeats(Flight& f) {
	cout << "\nSeat Layout (E=Empty, B=Booked)\n";
	cout << "   ";
	for (int c = 0; c < f.cols; c++)
		cout << c + 1 << " ";
	cout << endl;
	for (int i = 0; i < f.rows; i++) {
		cout << i + 1 << "  ";
		for (int j = 0; j < f.cols; j++)
			cout << f.seats[i][j] << " ";
		cout << endl;
	}
}
void cancel_booking()
{
	int flightID;
	bool found = false;
	int flightIndex, bookingIndex = -1;
	cout << "\nEnter Flight ID to cancel booking: ";
	cin >> flightID;
	for (int i = 0; i < passenger[userIndex].bookingCount; i++) {
		if (passenger[userIndex].bookings[i].id == flightID) {
			bookingIndex = i;
			break;
		}
	}
	if (bookingIndex == -1) {
		cout << "No Bookings Found for this flight number." << endl;
		return;
	}
	for (int i = 0; i < flightCount; i++) {
		if (flight[i].id == flightID) {
			flightIndex = i;
			found = true;
			break;
		}
	}

	if (!found) {
		cout << "Invalid Flight ID!" << endl;
		return;
	}

	if (flight[flightIndex].booked <= 0) {
		cout << "No bookings to cancel!" << endl;
		return;
	}
	int r = passenger[userIndex].bookings[bookingIndex].seatRow - 1;
	int c = passenger[userIndex].bookings[bookingIndex].seatCol - 1;
	flight[flightIndex].seats[r][c] = 'E';

	if (flight[flightIndex].booked > 0)
		flight[flightIndex].booked--;

	Booking* temp = new Booking[passenger[userIndex].bookingCount - 1];
	for (int i = 0, j = 0; i < passenger[userIndex].bookingCount; i++) {
		if (i != bookingIndex) {
			temp[j] = passenger[userIndex].bookings[i];
			j++;
		}
	}
	delete[] passenger[userIndex].bookings;
	passenger[userIndex].bookings = temp;
	passenger[userIndex].bookingCount--;

	saveFlights();
	saveBookings();

	cout << "\nBooking Cancelled Successfully!" << endl;
}
void generate_report() {
	if (passenger[userIndex].bookingCount == 0) {
		cout << "\n---------------------------------------\n";
		cout << "No Booking history found." << endl;
		cout << "\n---------------------------------------";
		return;
	}

	double total = 0;
	cout << "\n------------------------------------\n";
	cout << "\tBooking Report";
	cout << "\n------------------------------------\n";
	for (int i = 0; i < passenger[userIndex].bookingCount; i++) {
		Booking b = passenger[userIndex].bookings[i];
		cout << i + 1 << ":\nFlight ID: " << b.id
			<< "\nFrom: " << b.origin
			<< "\tTo: " << b.destination
			<< "\nDate: " << b.date
			<< "\nClass: " << b.cls
			<< "\nSeat: Row " << b.seatRow << " Seat " << b.seatCol
			<< "\tFare Paid: " << b.fare << " PKR\n";
		total += b.fare;
	}
	cout << "\nTotal Expenditure: " << total << " PKR" << endl;
	cout << "\n--------------------------------------" << endl;
}

void admin_login()
{
	string user_name, password;
	bool run = true;
	do {
		cout << "---------------------------------------" << endl;
		cout << "\tAdmin\n";
		cout << "---------------------------------------" << endl;
		cout << "Enter the Username:";
		cin >> user_name;
		cout << "Enter the Password:";
		cin >> password;
		if (password == adminPass && user_name == adminUser) {
			cout << "Access Granted!" << endl;
			admin();
			run = false;
		}
		else
			cout << "Wrong username or Password!" << endl;
	} while (run);
}
void admin() {
	int choice;
	bool run = true;

	do {
		cout << "\n\t Welcome to Admin Menu" << endl;
		cout << "1. Change Password" << endl;
		cout << "2. Show Registered Flights" << endl;
		cout << "3. Add Flight" << endl;
		cout << "4. Remove Flight" << endl;
		cout << "5. Add Passenger" << endl;
		cout << "6. Remove Passenger" << endl;
		cout << "7. Update Passenger Details" << endl;
		cout << "8. Passenger Reports" << endl;
		cout << "9. Flight Reports" << endl;
		cout << "10. View Current Passengers" << endl;
		cout << "0. Log Out" << endl;
		cout << "Enter choice: ";
		cin >> choice;

		switch (choice) {
		case 1: changePassword(); break;
		case 2: viewRegisteredFlights(); break;
		case 3: addFlight(); break;
		case 4: removeFlight(); break;
		case 5: addPassenger(); break;
		case 6: removePassenger(); break;
		case 7: updatePassengerDetails(); break;
		case 8: checkReportsPassengers(); break;
		case 9: checkReportsFlights(); break;
		case 10: viewCurrentPassengers(); break;
		case 0:
			cout << "Logging out..." << endl;
			run = false;
			break;
		default:
			cout << "Invalid Input" << endl;
		}
	} while (run);
}
void changePassword()
{
	string pass, updated_password;
	do {
		cout << "Enter old password: " << endl;
		cin >> pass;
		if (pass == adminPass)
			cout << "Now you can change the Password" << endl;
		else
			cout << "Wrong Password!" << endl;
	} while (pass != adminPass);
	cout << "Enter new Password: ";
	cin >> updated_password;
	adminPass = updated_password;
	cout << "Password Changed Successfully!" << endl;
	cout << endl;
}
void viewRegisteredFlights() {
	if (flightCount == 0) {
		cout << "\n---No flights are currently registered---" << endl;
		return;
	}

	cout << "\n==============================================================" << endl;
	cout << "\t\tREGISTERED FLIGHTS" << endl;
	cout << "==============================================================" << endl;
	for (int i = 0; i < flightCount; i++) {
		cout << "Flight #" << (i + 1) << " [ID: " << flight[i].id << "]" << endl;
		cout << "  Route:      " << flight[i].origin << " to " << flight[i].destination << endl;
		cout << "  Date:       " << flight[i].date << endl;
		cout << "  Time:       Dept: " << flight[i].departTime << " | Arrv: " << flight[i].arrivalTime << endl;
		cout << "  Seats:      Total: " << flight[i].total << " | Booked: " << flight[i].booked;
		int remaining = flight[i].total - flight[i].booked;
		cout << " | Left: " << remaining << endl;
		cout << "  Fares:      Eco: " << flight[i].economy << " PKR | Biz: " << flight[i].business << " PKR" << endl;
		cout << "--------------------------------------------------------------" << endl;
	}
}
void saveFlights() {
	ofstream file("Flights.txt");
	if (!file) return;

	for (int i = 0; i < flightCount; i++) {
		file << flight[i].id << " "
			<< flight[i].origin << " "
			<< flight[i].destination << " "
			<< flight[i].date << " "
			<< flight[i].departTime << " "
			<< flight[i].arrivalTime << " "
			<< flight[i].total << " "
			<< flight[i].booked << " "
			<< flight[i].economy << " "
			<< flight[i].business << " "
			<< flight[i].rows << " "
			<< flight[i].cols << " ";
		for (int j = 0; j < flight[i].rows; j++)
			for (int k = 0; k < flight[i].cols; k++)
				file << flight[i].seats[j][k] << " ";
		file << endl;
	}
	file.close();
}
void addFlight() {
	if (flightCount == fcapacity) {
		fcapacity += 5;
		Flight* temp = new Flight[fcapacity];
		for (int i = 0; i < flightCount; i++)
			temp[i] = flight[i];
		delete[] flight;
		flight = temp;
	}
	Flight f;
	cout << "\n--- Add New Flight ---" << endl;
	cout << "Flight ID: "; cin >> f.id;
	cout << "Origin: "; cin >> f.origin;
	cout << "Destination: "; cin >> f.destination;
	cout << "Date (DD-MM-YYYY): "; cin >> f.date;
	cout << "Departure Time: "; cin >> f.departTime;
	cout << "Arrival Time: "; cin >> f.arrivalTime;
	cout << "Enter number of rows: "; cin >> f.rows;
	cout << "Enter number of seats per row: "; cin >> f.cols;
	f.seats = new char* [f.rows];
	for (int i = 0; i < f.rows; i++) {
		f.seats[i] = new char[f.cols];
		for (int j = 0; j < f.cols; j++)
			f.seats[i][j] = 'E';
	}
	f.total = f.rows * f.cols;
	cout << "Economy Fare: "; cin >> f.economy;
	cout << "Business Fare: "; cin >> f.business;
	f.booked = 0;

	flight[flightCount] = f;
	flightCount++;

	saveFlights();
	cout << "Flight added and file has been updated" << endl;
}
void removeFlight() {
	if (flightCount == 0) {
		cout << "No flights to remove." << endl;
		return;
	}

	int idToRemove, foundIndex = -1;
	cout << "Enter Flight ID to remove: ";
	cin >> idToRemove;

	for (int i = 0; i < flightCount; i++) {
		if (flight[i].id == idToRemove) {
			foundIndex = i;
			break;
		}
	}

	if (foundIndex != -1) {
		for (int i = foundIndex; i < flightCount - 1; i++) {
			flight[i] = flight[i + 1];
		}
		flightCount--;
		saveFlights();
		cout << "Flight " << idToRemove << " removed successfully." << endl;
	}
	else {
		cout << "Flight ID not found." << endl;
	}
}

void addPassenger() {
	Passenger p;
	p.id = 1000 + passengerCount;
	p.bookingCount = 0;
	p.bookings = nullptr;
	cout << "\n--- Add Passenger ---" << endl;
	cout << "First Name: "; cin >> p.firstName;
	cout << "Last Name: "; cin >> p.lastName;
	cout << "Gender (M/F): "; cin >> p.gender;
	cout << "Email: "; cin >> p.email;
	cout << "Phone: "; cin >> p.phone;
	cout << "Set Passenger password: "; cin >> p.password;
	for (int i = 0; i < passengerCount; i++) {
		if (passenger[i].email == p.email) {
			cout << "Passenger already exists." << endl;
			return;
		}
	}
	if (passengerCount == pcapacity) {
		pcapacity += 5;
		Passenger* temp = new Passenger[pcapacity];
		for (int i = 0; i < passengerCount; i++)
			temp[i] = passenger[i];
		delete[] passenger;
		passenger = temp;
	}
	passenger[passengerCount] = p;
	appendPassenger();
	passengerCount++;
	cout << "Passenger added successfully" << endl;
	cout << "Passenger ID: " << p.id << endl;
	cout << "Password: " << p.password << endl;
}
void removePassenger() {
	int id;
	cout << "Enter Passenger ID to Remove: ";
	cin >> id;

	int index = -1;
	for (int i = 0; i < passengerCount; i++) {
		if (passenger[i].id == id) {
			index = i;
			break;
		}
	}

	if (index == -1) {
		cout << "Passenger not found" << endl;
		return;
	}

	delete[] passenger[index].bookings;

	for (int i = index; i < passengerCount - 1; i++)
		passenger[i] = passenger[i + 1];

	passengerCount--;
	savePassenger();

	cout << "Passenger removed successfully" << endl;
}
void updatePassengerDetails() {
	string email;
	cout << "Enter Passenger Email: ";
	cin >> email;

	for (int i = 0; i < passengerCount; i++) {
		if (passenger[i].email == email) {
			cout << "New Phone: ";
			cin >> passenger[i].phone;
			cout << "New Password: ";
			cin >> passenger[i].password;

			savePassenger();

			cout << "Passenger updated successfully!" << endl;
			return;
		}
	}
	cout << "Passenger not found!" << endl;
}
void checkReportsPassengers() {
	cout << "\n--- Passenger Report ---" << endl;
	for (int i = 0; i < passengerCount; i++) {
		double total = 0;
		cout << "\nPassenger: " << passenger[i].firstName << " "
			<< passenger[i].lastName << "\n" << passenger[i].id << "\n";

		for (int j = 0; j < passenger[i].bookingCount; j++) {
			Booking b = passenger[i].bookings[j];
			cout << "  Flight ID: " << b.id
				<< " | " << b.origin << " -> " << b.destination
				<< " | Fare: " << b.fare << " PKR\n";
			total += b.fare;
		}

		cout << "  Total Spent: " << total << " PKR\n";
		cout << "----------------------------------\n";
	}
}
void checkReportsFlights() {
	cout << "\n--- Flight Inventory Report ---\n";
	for (int i = 0; i < flightCount; i++) {
		int total = 0;
		int count = 0;
		for (int j = 0; j < passengerCount; j++) {
			for (int k = 0; k < passenger[j].bookingCount; k++) {
				if (passenger[j].bookings[k].id == flight[i].id) {
					count++;
					total += passenger[j].bookings[k].fare;
				}
			}
		}
		int available = flight[i].total - flight[i].booked;
		string status;
		if (available > 0)
			status = "Available";
		else
			status = "Full";
		cout << "\nFlight ID: " << flight[i].id
			<< "\nRoute: " << flight[i].origin << " -> " << flight[i].destination
			<< "\nTimes Booked: " << count
			<< "\nTotal Passengers carried: " << flight[i].booked
			<< "\nAvailable Seats: " << available
			<< "\nStatus: " << status
			<< "\nIncome Generated: " << total << " PKR\n"
			<< "\n-------------------------------\n";
	}
}
void viewCurrentPassengers() {
	if (passengerCount == 0) {
		cout << "\nNo passengers found.\n";
		return;
	}

	cout << "\n================== Current Passengers ==================\n";
	for (int i = 0; i < passengerCount; i++) {
		cout << i + 1 << ". Name: " << passenger[i].firstName << " " << passenger[i].lastName
			<< " | User ID: " << passenger[i].id
			<< " | Gender: " << passenger[i].gender
			<< " | Email: " << passenger[i].email
			<< " | Phone: " << passenger[i].phone
			<< " | Bookings: " << passenger[i].bookingCount << endl;
	}
	cout << "========================================================\n";
}
