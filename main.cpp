#include <iostream>
#include <string>
#include <fstream>
#include <istream>
using namespace std;

void static registration(string username, string password) {
	/*  
	x++;
	if (x == 100){
	cout << "The amount of registered user is limited to 100!";
	break;
	}
	*/
	ofstream credentials("credentials.txt", std::ios::app);
	credentials << username << ", " << password << "\n";
	credentials.close();
	cout << "Successfully registered as student!";
}

void static adminlogin(string username, string password) {
	if (username == "admin",password == "admin") {
		cout << "Welcome admin!";
	}
}

void static studentlogin(string username, string password) {
	fstream credentials("credentials.txt", ios::in);
	if (credentials.is_open()) {
		string sa;
		while (getline(credentials, sa, ', '));
	}
}

int main() {
	int x = 0;
	while (x == 0){
		int choice;
		string username;
		string password;
		cout << "Welcome to the Trivia Persuit DSTR Ed.\n";
		cout << "Choose option :\n 1. Login as Student\n2. Login as Teacher\n3. Register as student\n4. Leave\n";
		cin >> choice;
		switch (choice) {
		case 1:
			cout << "Enter login";
			cin >> username;
			cout << "Enter password";
			cin >> password;
			studentlogin(username, password);
		case 2:
			cout << "Enter login";
			cin >> username;
			cout << "Enter password";
			cin >> password;
			adminlogin(username, password);
		case 3:
			cout << "Enter login\n";
			cin >> username;
			cout << "Enter password\n";
			cin >> password;
			registration(username, password);
		case 4:
			cout << "Goodbye!";
			x++;
		}
	}
}

