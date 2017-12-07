#include "socketserver.h"
#include "socket.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <string>
#include <thread>
#include <iostream>
#include "Semaphore.h"
#include "SharedObject.h"


using namespace Sync;
using namespace std;

Semaphore* mutex = new Semaphore("accessToSharedObject",1);
Semaphore* alert= new Semaphore("signalThreads",0);


vector<thread> threads;
vector<Socket> clientSockets;
bool running = true; //to determine if server is running


void modifyString(int counter) {

        ByteArray fromClient;
        //Socket &mySocket = sockets[count];//make a temp socket address and reference it to the socket made earlier

        while (running) {

                //alert->Wait();
                //mutex->Wait();
                Socket& myClient = clientSockets[counter];
                Socket otherPlayerSocket = clientSockets[1];

                int tempInt = myClient.Read(fromClient);//read the bytearray data sent from client
                //convert and store as string
                string temp = fromClient.ToString();

                if (counter == 0) {
                    otherPlayerSocket = clientSockets[1];
                } else if(counter ==1){
                    otherPlayerSocket = clientSockets[0];
                }
                if (temp == "done"|| tempInt <=0) {
                    myClient.Close();
                    running == false;
                    return;//get out of loop
                } else {

                    cout<<"Sending message to player "<<counter+1<<" "<<temp<<endl;
                    otherPlayerSocket.Write(ByteArray(temp)); //send back the converted string to client as a ByteArray
                }
                //mutex->Signal();
        }
}

int main(void)
{

    int playerCounter = 0;
    SocketServer myServer = SocketServer(2000);

    cout << "I am a server."<<endl;

    while (running){
        cout<< "reading.."<< endl;

        Socket temp = myServer.Accept();//accept data from socket, blocks until accepted

        clientSockets.push_back(temp); //add it to the vector of sockets
        cout <<"Player thread "<<playerCounter<<" created and running"<<endl;

        threads.push_back(thread(modifyString, playerCounter));// add thread to the vector of threads
        playerCounter++;//increment player counter
    }

    for (int i = 0; i <threads.size(); i++){
        cout<<"Releasing Threads...."<<i<<endl;
        threads[i].join();
    }
    myServer.Shutdown();


}
