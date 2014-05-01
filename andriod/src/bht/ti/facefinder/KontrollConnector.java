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
	
	
	public KontrollConnector(String ip, int Port) {
		// TODO Auto-generated constructor stub
		setIP(ip);
		setPort(Port);		
		connect();
	}
	
	public void connect() {
		new Thread(new ClientThread()).start();
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
	
		@Override
		public void run() {
			// TODO Auto-generated method stub
			
			try {
				socket = new Socket(getIP(),getPort());
				PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
				out.println("test");
				
				try{
					BufferedReader br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
					String read = br.readLine();
					Log.i("MY", "Socket Read: " + read);
				}
				catch(IOException e) {
	                e.printStackTrace();	
				}
				
				socket.close();
			
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
