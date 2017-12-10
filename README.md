# Boxing
Boxing Game using threads and server client architecture

# Build Instructions Windows

Locate KeyPairs folder (Boxing/Master/Keypairs) and save to C:/ 

open cmd and run:
bvssh -keypairFile=c:\KeyPairs\abaggio2013.pem -user=abaggio -host=ec2-54-167-215-132.compute-1.amazonaws.com
press login on Bitvise SSH Client

In Unix terminal

cd Master/Backend

make

./Server


Open two of the following to play:


  Open another Unix terminal, 
  
  cd Master/Backend
  
  ./Client



  OR


  Open Boxing/Frontend in Android Studio, 
  
  Build the main activity to your device or emulator 
  
  Run
  

Play on Unix:

type anything to "punch"

type quit to exit


Play on device:

Connect Button, connects to a new game

Quit Button, quitc current game

Punch Button throws a punch
