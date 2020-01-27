#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<Windows.h>
#include<vector>
#include<iostream>
#include<string>
#include"SingleTon.h"
#include"BullyLogic.h"

using namespace std;
#pragma comment(lib,"Ws2_32.lib")
#define PPORT "1234"
#define BUFFER_SIZE 512

class BullyProcess {
	WSADATA wsadata;
	SOCKET ISocket;
	DWORD ThreadID;
public:
	BullyLogic* bll;
	class BullyProcessInfo {
	public:
		BullyProcessInfo(SOCKET sk, int st, BullyLogic* bl) {Socket = sk; status = st; handle = NULL; singletonProcesses = Singleton::getInstance(); this->bl =bl; }
		SOCKET Socket;
		int status;
		HANDLE handle;
		Singleton* singletonProcesses;
		BullyLogic* bl;
	};
	std::vector<BullyProcessInfo> BullyProcesss;
public:
	BullyProcess() {
		struct addrinfo *result = NULL, ref;
		wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
		bll = new BullyLogic();
		ZeroMemory(&ref, sizeof(ref));
		ref.ai_family = AF_INET;
		ref.ai_socktype = SOCK_STREAM;
		ref.ai_protocol = IPPROTO_TCP;
		ref.ai_flags = AI_PASSIVE;
		getaddrinfo("127.2.2.2", PPORT, &ref, &result);	
		ISocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		bind(ISocket, result->ai_addr, result->ai_addrlen);
	}

	void Listen() {
		do
		{
			listen(ISocket, SOMAXCONN);
			SOCKET CSocket=INVALID_SOCKET;
			CSocket = accept(ISocket, NULL, NULL);
			if(CSocket!=INVALID_SOCKET)BullyProcesss.push_back(BullyProcessInfo(CSocket, 1,this->bll));
			UpDateBullyProcess();
		} while (true);	
	}

	void takeMessageFromProceesToOther(string msg) {
		struct addrinfo *result = NULL, ref;
		ZeroMemory(&ref, sizeof(ref));
		getaddrinfo("127.2.2.2", PPORT, &ref, &result); 
		SOCKET Ani=socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		connect(Ani, result->ai_addr, result-> ai_addrlen);
		int status;
		status = send(Ani, msg.c_str(), msg.length(), 0);
	}

	void UpDateBullyProcess() {
		for (int i = 0; i < BullyProcesss.size(); i++) {
			if (BullyProcesss[i].status==1) {
				if(BullyProcesss[i].handle==NULL)BullyProcesss[i].handle=CreateThread(0,0,Receive,&BullyProcesss[i],0,&ThreadID);
			} else {
				BullyProcesss.erase(BullyProcesss.begin() + i); //point to curernet
			}
		}
	}

	static DWORD WINAPI Receive(void* bullyProcess) {
		BullyProcessInfo* BInfo = (BullyProcessInfo*)bullyProcess;
		int status;
		char revBuffer[BUFFER_SIZE];
		do {
			memset(&revBuffer[0], 0, sizeof(revBuffer));
			status=recv(BInfo->Socket, revBuffer, BUFFER_SIZE, 0);
			if (status > 0) {
				BInfo->bl->didRecivedMessage(revBuffer);
			} else {
				break;
			}
		} while (true);
		shutdown(BInfo->Socket, SD_SEND);
		closesocket(BInfo->Socket);			
		BInfo->status = 0;
		return 0;
		}

	~BullyProcess() {
		for each (BullyProcessInfo info in BullyProcesss) {
			CloseHandle(info.handle);
		}
		WSACleanup();
	}
};
