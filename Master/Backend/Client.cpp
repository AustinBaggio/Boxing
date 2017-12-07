#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "Semaphore.h"
#include "SharedObject.h"

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
	Socket s ("127.0.0.1", 2000);

	//connect to IP address and port specified
    s.Open();

	while (true){

        //mutex->Wait();

        if (userInput == "done") {
            cout << "terminating...." << endl;
            break;
        }

		cout <<"type: punch"<<endl;
		getline(cin, userInput);

        //transforms string into raw data
        ByteArray rawUserInput = (userInput);

        //sends the raw data
        try {
            s.Write(rawUserInput);
        } catch (int e) {
            cout <<"data failed to send"<<endl;
            continue;
        }

		//alert->Signal();
        //mutex->Signal();
        //record data being received to the bytearray object
		s.Read(msgReceived);

        health-=1;
		//display the received message in string format
		cout <<"Got punched! Health is now: "<<health<<endl;

        if (health == 0) {
            cout <<"I'm dead, game over"<<endl;
        }

	}
	s.Close();//close the socket when loop breaks
}
