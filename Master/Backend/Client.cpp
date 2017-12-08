#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "Semaphore.h"
#include "SharedObject.h"
#include "Blockable.h"

using namespace Sync;
using namespace std;

Semaphore* mutex = new Semaphore("accessToSharedObject",1,true);
Semaphore* alert= new Semaphore("signalThreads",0,true);

int main(void)
{
	int health = 10;
    string userInput;
	ByteArray msgReceived;

	std::cout << "I am a player" << std::endl;

	//construct a socket
	Socket s ("127.0.0.1", 2002);

	//connect to IP address and port specified
    s.Open();
    bool playGame = true;

	while (playGame){

        FlexWait waiter(2,&s,&cinWatcher);
        Blockable* result = waiter.Wait();
        //mutex->Wait();

        

		//cout <<"type: punch"<<endl;
        if(result == &cinWatcher){
		getline(cin, userInput);
        
        //transforms string into raw data
        ByteArray rawUserInput = (userInput);

        //sends the raw data
        try {
            s.Write(rawUserInput);
        } catch (...) {
            cout <<"data failed to send"<<endl;
            continue;
        }
        if (userInput == "done") {
            cout << "terminating...." << endl;
            playGame=false;
            break;
        }
        }
		//alert->Signal();
        //mutex->Signal();
        //record data being received to the bytearray object
    
        if(result==&s){


		s.Read(msgReceived);
        string temp = msgReceived.ToString();
        std::cout <<temp<<endl;


       // health-=1;
		//display the received message in string format
		//cout <<"Got punched! Health is now: "<<health<<endl;

        if (health == 0) {
            
            playGame=false;
        }
        }
	}
	s.Close();//close the socket when loop breaks
}
