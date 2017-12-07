#include "socketserver.h"
#include "socket.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <string>
#include <thread>
#include <iostream>
#include <mutex>
#include "Semaphore.h"
#include "SharedObject.h"


using namespace Sync;
using namespace std;

Semaphore* mutex = new Semaphore("accessToSharedObject",1);
Semaphore* alert= new Semaphore("signalThreads",0);


vector<thread> threads;
bool running = true; //to determine if server is running


void modifyString(Socket s, int counter) {

        ByteArray fromClient;
        //Socket &mySocket = sockets[count];//make a temp socket address and reference it to the socket made earlier

        while (running) {

                alert->Wait();
                mutex->Wait();
                int tempInt = s.Read(fromClient);//read the bytearray data sent from client
                //convert and store as string
                string temp = fromClient.ToString();

                if (temp == "done"|| tempInt <=0) {
                    s.Close();
                    running == false;
                    return;//get out of loop
                } else {
                    for (int i = 0; i < temp.length(); i++) {
                        temp[i] = toupper(temp[i]); //converts string to all uppercase version
                    }
                    cout<<"Sending message to player "<<counter<<endl;
                    s.Write(ByteArray(temp)); //send back the converted string to client as a ByteArray
                }
                mutex->Signal();
        }
}

int main(void)
{

    int playerCounter = 0;
    SocketServer myServer = SocketServer(2000);

    cout << "I am a server."<<endl;

    while (running){
        cout<< "reading.."<< endl;

        Socket s1 = myServer.Accept();//accept data from socket, blocks until accepted
        cout <<"Player thread "<<playerCounter<<" created and running"<<endl;
        playerCounter++;
        threads.push_back(thread(modifyString, s1, playerCounter));// add thread to the vector of threads
    }

    for (int i = 0; i <threads.size(); i++){
        cout<<"Releasing Threads...."<<i<<endl;
        threads[i].join();
    }
    myServer.Shutdown();


}
