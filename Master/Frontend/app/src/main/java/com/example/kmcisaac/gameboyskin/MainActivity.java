package com.example.kmcisaac.gameboyskin;

import android.os.Bundle;
import android.os.Handler;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.EditText;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //create the main layout
        setContentView(R.layout.activity_main);
        theSocket = null;

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    //Declare sockets, handlers and UI Elements
    private Socket theSocket;
    private OutputStream outStream;
    private InputStream inStream;
    private Handler uiThread;
    private String rxText;
    private ConnectionThread theConnection;
    private EditText userIn;


    //Gettinga connection using mutiple threads
    public class ConnectionThread extends Thread
    {
        public ConnectionThread()
        {

        }
        public void run()
        {
            try
            {
                //Connect the socket to the AWS server
                theSocket = new Socket(InetAddress.getByName("54.167.215.132"), 2013);

                //Get the input and output streams
                outStream = theSocket.getOutputStream();
                inStream = theSocket.getInputStream();

                //Update the UI
                uiThread.post(new Runnable(){
                    public void run()
                    {
                        BlankCanvas bc = findViewById(R.id.canvas1);
                        bc.setText("Connected");
                    }
                });

                //Ready string in a byte ibject
                byte[] readBuffer = new byte[256];
                while(true)
                {

                    //send to Socket
                    for (int i=0;i<256;i++)
                        readBuffer[i]=0;
                    inStream.read(readBuffer);
                    rxText = new String(readBuffer, "UTF-8");

                    //update the response onto the UI
                    uiThread.post(new Runnable(){
                        public void run()
                        {
                            BlankCanvas bc = findViewById(R.id.canvas1);
                            bc.setText("Got:" + rxText);
                        }
                    });
                }
            }
            catch (Exception e)
            {
                System.out.println(e.toString());
            }
        }
    }

    //Connect to a new game
    public void onConnectButton(View v)
    {
        try {
            uiThread = new Handler();
            theConnection = new ConnectionThread();
            theConnection.start();
        }
        catch(Exception e)
        {
            Log.d("Error",e.toString());
        }
    }

    //Send helper function
    private class Sender extends Thread {
        String toSend;
        Sender (String s)
        {
            toSend = s;
        }
        public void run()
        {
            if (theSocket == null) {
                Log.d("Null Socket toSend: ", toSend);
                return;
            }

            //If success
            try {
                Log.d("sent Success: ", toSend);

                outStream.write(toSend.getBytes());
            }
            catch(Exception e)
            {
                Log.d("Error",e.toString());
            }
        }
    }

    //User Input functions

    public void onPunchButton(View v)
    {
        userIn = (EditText) findViewById(R.id.userIn);

        //Old Send
        //new Sender("Punch").start();

        new Sender(userIn.getText().toString()).start();
    }

    public void OnBlockButton(View v)
    {
        new Sender("Block").start();
    }
    public void onDisconnectButton(View v)
    {
        new Sender("quit").start();

    }
}
