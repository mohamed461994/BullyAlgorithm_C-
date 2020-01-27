#include"ProcessModel.h"
#include<string>
#include<iostream>
using namespace std;

class Singleton
{
	
    private:
        static Singleton* instance;
        Singleton();
    public:
        static Singleton* getInstance();
		ProcessX allProcessesModel[30];
		int processCount;
};

Singleton* Singleton::instance = 0;

Singleton* Singleton::getInstance()
{
    if (instance == 0) {
        instance = new Singleton();
		
    }
    return instance;
}

Singleton::Singleton()
{processCount = 0;}