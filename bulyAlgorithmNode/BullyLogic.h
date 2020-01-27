#include<iostream>
#include <string>
using namespace std;


class BullyLogic {
	//Process allProcessesModel[30];
	Singleton* singletonProcesses;
	int processCount;
public:
	BullyLogic() {processCount = 0; singletonProcesses = Singleton::getInstance(); }

	void didRecivedMessage(string msg) {
		string newString = msg;
		if (msg[newString.size()-1] == 'u') { // if it's end by u then it become up
			newString = newString.substr(0,newString.size()-1);
			
			processStatusChanged(newString, false);
			int processID = stoi(newString);
			activeProcess(processID);
		} else if (msg[newString.size()-1] == 'd') { // if it's end by d then it become Down
			newString = newString.substr(0,newString.size()-1);
			processStatusChanged(newString,true);
			int processID = stoi(newString);
			deActiveProcess(processID);
		} else {
			newProcessUpdate(newString);
			addNewProcess(stoi(newString));
			singletonProcesses->processCount++;
		}

		startElection();
	}

	void newProcessUpdate(string procesID) {
		print("---------------------new process----------------");
		print("process ID is: "+procesID);
		print("-------------------------------------");
	}

	void processStatusChanged(string procesID, bool isDown) {

		for (int i = 0; i < singletonProcesses->processCount; i++) {
			if (singletonProcesses->allProcessesModel[i].id == stoi(procesID)) {
				singletonProcesses->allProcessesModel[i].status = !isDown;
			}
		}

		string downOrUpString = isDown ? "Down" : "up";
		print("---------------------new process----------------");
		print("process ID is: "+procesID+" become "+downOrUpString);
		print("-------------------------------------");
	}

	void printAllProcessesAndItsStatus() {
		for (int i = 0; i < singletonProcesses->processCount; i++) {
			int proID = singletonProcesses->allProcessesModel[i].id;
			cout<< proID;
			cout<< "\n";
		}
	}

	void startElection() {
		if (singletonProcesses->processCount == 1) { // if only one process
			int proID = singletonProcesses->allProcessesModel[0].id;
			annonceCordinator(proID);
		}

		int cordiinatorID = singletonProcesses->allProcessesModel[0].id ;
		for (int i = 0; i < singletonProcesses->processCount; i++) {
			if (singletonProcesses->allProcessesModel[i].id > cordiinatorID && singletonProcesses->allProcessesModel[i].status) {
				cordiinatorID = singletonProcesses->allProcessesModel[i].id;
			}
			for (int j = 0; j < singletonProcesses->processCount; j++) {
				if (singletonProcesses->allProcessesModel[i].status) {//if its not Down
					if (singletonProcesses->allProcessesModel[i].id < singletonProcesses->allProcessesModel[j].id) {
						processComunication(singletonProcesses->allProcessesModel[i].id, singletonProcesses->allProcessesModel[j].id);						
					}
				}
			}
		}
		annonceCordinator(cordiinatorID);
	}



	void processComunication(int process1ID, int process2ID) {
		cout<< "Process with id = " <<process1ID<<" is electing procees with id "<<process2ID<<"\n";
	}

	void annonceCordinator(int procesID) {
		print("-------------------------------------");
		cout<< "process Cordinator is: ";
		cout<< procesID;
		print("\n-------------------------------------");
	}

	void addNewProcess(int processID) {
		if (processCount >= 30 ) {
			print("MAX SIZE");
		} else {
			ProcessX p;
			p.id = processID;
			p.status = true;
			p.isCordinator = false;
			singletonProcesses->allProcessesModel[processCount] = p;
			processCount++;
		}
	}

	void deActiveProcess(int processID) {
		changeProseesStatus(processID, false);
		/*
		for (int i = 0; i <  processCount; i++) {
			if (allProcessesModel[i].id == processID) {
				allProcessesModel[i].status = false;
			}
		}
		*/
	}

	void activeProcess(int processID) {
		 changeProseesStatus(processID, true);
		/*
		for (int i = 0; i <  processCount; i++) {
			if (allProcessesModel[i].id == processID) {
				allProcessesModel[i].status = true;
			}
		}
		*/
	}
	
	void print(string input) {
		cout << input << "\n";
	}

private:
	void changeProseesStatus(int processID, bool status) {
		for (int i = 0; i <  processCount; i++) {
			if (singletonProcesses->allProcessesModel[i].id == processID) {
				singletonProcesses->allProcessesModel[i].status = status;
			}
		}
	}

};
