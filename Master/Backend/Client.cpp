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
    ByteArray msgReceived;
    bool ready = false;
    bool quit=false;

    std::cout << "I am a player" << std::endl;

    //construct a socket
    Socket s ("54.167.215.132", 2013);

    //connect to IP address and port specified
    s.Open();

    /*
    while (!ready){

        s.Read(msgReceived);

        if(msgReceived.ToString() == "Waiting for opponent..."){
            cout<<msgReceived.ToString();
            continue; //continue to loop until message is not "waiting for opponent"
        } else {
            cout<<msgReceived.ToString(); //this message should be "Ready"
            ready = true;
        }
    }*/

    while (!quit){

        FlexWait waiter(2, cinWatcher, &s);
        Blockable * result = waiter.Wait(); //flex wait starts

        if (result == &s){

            s.Read(msgReceived);
            cout<<msgReceived.ToString()<<endl;
            if (msgReceived.ToString() =="quit"){
                quit = true;
                break;
            }

        } else if (result == &cinWatcher) {
            string input;
            getline(cin, input);//get input from user

            if (input =="quit"){
                s.Write(ByteArray(input)); //tells server that user has quit
                quit = true;
                break; //end the client
            }

            s.Write(ByteArray(input)); //if it didn't end, write it to the socket
        }
    }
    cout<<"Quiting..."<<endl;
    s.Close();//close the socket when loop breaks
}
