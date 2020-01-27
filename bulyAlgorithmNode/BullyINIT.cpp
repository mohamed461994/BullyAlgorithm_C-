#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<iostream>
#include<string>
#include"BullyProcess.h"
//#include"BullyLogic.h"
#include<iostream>
using namespace std;
#pragma comment(lib,"Ws2_32.lib")
#define PPORT "1234"
#define BUFFER_SIZE 512

int main() {
	cout<<"IS This your main process please enter small y if yes or any if no\n";
	string input = "";
	getline(cin, input);
	BullyProcess process;
	
	if (input[0] == 'y') {
		process.Listen();
    } 
	cout<<"please enter process ID Int\n ";
	int currentProcessID;
	cin >> currentProcessID;
	process.takeMessageFromProceesToOther(to_string(currentProcessID));
	do {
		cout<< "DO You want This process Down please enter small y if yes\n";
		getline(cin, input);
		if (input[0] == 'y') {
			process.takeMessageFromProceesToOther(to_string(currentProcessID)+"d");	// if start with d its down
			cout<<"Process now is Down if you want process up please enter small y else enter any \n";
			getline(cin, input);
			if (input[0] == 'y') {
				process.takeMessageFromProceesToOther(to_string(currentProcessID)+"u"); // if start with u its back to up
			}
		} 
	} while (true);



	WSACleanup();
	return 0;

	
}