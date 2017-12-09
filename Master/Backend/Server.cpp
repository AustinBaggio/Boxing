#include "socketserver.h"
#include "socket.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <string>
#include <thread>
#include <pthread.h>
#include <iostream>
#include "Semaphore.h"
#include "SharedObject.h"


using namespace Sync;
using namespace std;

Semaphore* mutex = new Semaphore("accessToSharedObject",1);
Semaphore* alert= new Semaphore("signalThreads",0);

vector <Socket> socketList;
vector <thread> gameControllerList;
vector <thread> playerThreadList;


bool quit = false;

void playerThreadActivity( Socket otherPlayer, Socket me) {
    ByteArray data;
    //bool running = true;

    while (1) {

        FlexWait messageWaiter(2, &otherPlayer, &me);
        Blockable *result = messageWaiter.Wait(); //flex wait starts

        if (result == &otherPlayer) {
            otherPlayer.Read(data);

            //if player sends quit
            if (data.ToString() == "quit") {
                cout << "one player has quit, chatroom shutting down" << endl;
                me.Write(data); //tells the other player to quit

                //player thread terminates
                break;
            }

            me.Write(data);//send message to other player

        } else if (result == &me) {
            me.Read(data);

            if (data.ToString() == "quit") {
                cout << "one player has quit, chatroom shutting down" << endl;
                otherPlayer.Write(data); //tells the other player to quit
                break;
            }

            otherPlayer.Write(data);// send message to other player
        }
    }
    cout << "closing sockets..." << endl;
    otherPlayer.Close();
    me.Close();

}

void gameControllerActivity(Socket A, Socket B){
    //make thread for player 1
    playerThreadList.emplace_back(thread(playerThreadActivity, A, B));
    //make thread for player 2
    playerThreadList.emplace_back(thread(playerThreadActivity, A, B));
}


int main(void)
{
    int playerCount = 0;

    SocketServer myServer = SocketServer(2013);
    cout<<"this is the main system server that creates games"<<endl;
    cout<<"type quit to terminate"<<endl;


    while (!quit){
        //flexwait that triggers if server gets a new socket, or if user types an input
        FlexWait waiter(2, &myServer, &cinWatcher);
        Blockable * result = waiter.Wait(); //flex wait starts

        //if server receives a connection
        if (result == &myServer) {

            try { //waits for a player/socket to connect

                //gets a new player conncetion and the socket, automatically opened
                Socket newPlayer = myServer.Accept();
                //add the new connected socket to the list
                socketList.push_back(newPlayer); //add it to the socket list

                //if the socket list has 2 players
                if (socketList.size() == 2) {
                    socketList.at(0).Write(ByteArray("Ready"));
                    socketList.at(1).Write(ByteArray("Ready"));
                    //message to server saying a new game has been created
                    cout << "2 new players have connected, creating new game!" << endl;

                    //make a new game controller thread, add it to the list of game controllers,  and pass in the two sockets
                    gameControllerList.push_back(thread(gameControllerActivity, socketList.at(0), socketList.at(1)));

                    //clear the list of sockets so that it can now accept 2 new ones
                    socketList.clear();

                    continue;

                } else {
                    //if it is only one player connected
                    socketList.at(0).Write(ByteArray("Waiting for opponent..."));
                    //at this point, code will loop back to beginning and be on flex wait()

                }
            } catch (TerminationException terminationException)
            {
                return terminationException;
            } catch (string error) {
                cout << std::endl << "[Error] " << error << std::endl;
                return 1;
            }

        } else if (result == &cinWatcher){
            //if the user inputs quit
            string s;
            getline(cin, s); //get the user input if they wanna quit
            if (s == "quit"){
                quit = true; //end the loop
                break;
            } else
            {
                continue; //loop again
            }

        }

    }

    myServer.Shutdown();


}
