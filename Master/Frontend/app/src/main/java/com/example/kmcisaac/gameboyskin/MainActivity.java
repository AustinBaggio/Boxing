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

import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
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
    private Socket theSocket;
    private OutputStream outStream;
    private InputStream inStream;
    private Handler uiThread;
    private String rxText;
    private ConnectionThread theConnection;
    public class ConnectionThread extends Thread
    {
        public ConnectionThread()
        {

        }
        public void run()
        {
            try
            {
                //CHANGE TO AWS SERVER
                theSocket = new Socket(InetAddress.getByName("54.167.215.132"), 2010);
                outStream = theSocket.getOutputStream();
                inStream = theSocket.getInputStream();
                uiThread.post(new Runnable(){
                    public void run()
                    {
                        BlankCanvas bc = findViewById(R.id.canvas1);
                        bc.setText("Connected");
                    }
                });
                byte[] readBuffer = new byte[256];
                while(true)
                {
                    for (int i=0;i<256;i++)
                        readBuffer[i]=0;
                    inStream.read(readBuffer);
                    rxText = new String(readBuffer, "UTF-8");

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
    public void onDisconnectButton(View v)
    {
        try {
            if (theSocket != null)
                theSocket.close();
            theSocket = null;
            BlankCanvas bc = findViewById(R.id.canvas1);
            bc.setText("Disconnected");
        }
        catch(Exception e)
        {
            Log.d("Error",e.toString());
        }
    }
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

    public void onPunchButton(View v)
    {
        new Sender("Punch").start();
    }

    public void OnBlockButton(View v)
    {
        new Sender("Block").start();
    }
}
