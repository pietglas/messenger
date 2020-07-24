#include <iostream>
#include <string>

using namespace std;

class Sender {

};

class Receiver {
public:
	void print_received() {
		
	}
};

int main () {
	string message;
	while (message != string("exit")) {
		getline(cin, message);
		cout << message << endl;
	}
}