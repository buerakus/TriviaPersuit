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
	credentials << username << "," << password << "\n";
	credentials.close();
	cout << "Successfully registered as student!" << endl;
}

void static adminlogin(string username, string password) {
	if (username == "admin" && password == "admin") {
		cout << "Welcome admin!" << endl;
	}
}

int static studentlogin(string username, string password) {
	fstream credentials("credentials.txt", ios::in);
	bool found = false;
	if (credentials.is_open()) {
		string u;
		string p;
		while (getline(credentials, u, ',') && getline(credentials, p)) {
			if (username == u) {
				found = true;
				if (password == p) {
					cout << "Welcome " << username << endl;
					credentials.close();
					return 0;
				}
			}
		}
	}
}

int main() {
	int x = 0;
	while (x == 0){
		int choice;
		string username;
		string password;
		cout << "Welcome to the Trivia Persuit DSTR Ed.\n";
		cout << "Choose option :\n1. Login as Student\n2. Login as Teacher\n3. Register as student\n4. Leave\n";
		cin >> choice;
		switch (choice) {
		case 1:
			cout << "Enter login : " << endl;
			cin >> username;
			cout << "Enter password : " << endl;
			cin >> password;
			studentlogin(username, password);
			break;
		case 2:
			cout << "Enter login : " << endl;
			cin >> username;
			cout << "Enter password : " << endl;
			cin >> password;
			adminlogin(username, password);
			break;
		case 3:
			cout << "Enter login : " << endl;
			cin >> username;
			cout << "Enter password : " << endl;
			cin >> password;
			registration(username, password);
			break;
		case 4:
			cout << "Goodbye!";
			x++;
			break;
		}
	}
}

