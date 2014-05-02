package bht.ti.facefinder;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import android.util.Log;

public class KontrollConnector {
	
	private String IP;
	private int Port;
	private Socket socket;
	private ClientThread client;
	private Thread runningThread;
	
	
	public KontrollConnector(String ip, int Port) {
		// TODO Auto-generated constructor stub
		setIP(ip);
		setPort(Port);		
		connect();
	}
	
	public void connect() {
		client = new ClientThread();
		runningThread = new Thread(client);
		runningThread.start();
	}	
	
	public void disconnect() {
		client.disconnect();
		
	}
	
	public int getPort() {
		return Port;
	}

	public void setPort(int port) {
		Port = port;
	}

	public String getIP() {
		return IP;
	}

	public void setIP(String iP) {
		IP = iP;
	}
	
	class ClientThread implements Runnable
	{
	
		public void disconnect(){
			try {
				socket.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		@Override
		public void run() {
			// TODO Auto-generated method stub
			
			try {
				socket = new Socket(getIP(),getPort());
				PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
				out.println("test");
				
				BufferedReader br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
				String read = br.readLine();
				Log.i("MY", "Socket Read: " + read);
				
				
				
			
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

}
