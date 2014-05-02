package bht.ti.facefinder;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.PipedReader;
import java.io.PipedWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import android.util.Log;

public class KontrollConnector implements Runnable{
	
	private String IP;
	private int Port;
	private Socket socket;
	//private ClientThread client;
	private Thread runningThread;
	private PipedWriter pw;
	private PipedReader pr;
	
	public KontrollConnector(String ip, int Port) {
		// TODO Auto-generated constructor stub
		setIP(ip);
		setPort(Port);		
		connect();
	}
	
	public void connect() {
		//client = new ClientThread();
		pr = new PipedReader();
		pw = new PipedWriter();
		try {
			pw.connect(pr);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		runningThread = new Thread(this);
		runningThread.start();
	}	
	
	public void disconnect() {
		try {
			socket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void sendKommando(String jsonString){
			
		try {
			pw.write(jsonString);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		// Stream befüllen
		
	}
	
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		try {
			socket = new Socket(getIP(),getPort());
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		PrintWriter out = null;
		
		try {
			out = new PrintWriter(socket.getOutputStream(), true);
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		int item;
		String tosend;
		while(socket != null)
		{	
			try {
			tosend = "";
				while ((item = pr.read()) != -1 && pr.ready())
				{
					System.out.print((char) item);
					tosend += (char)item;

				}
				out.println(tosend);
				Log.d("MY", "Socket Send: " + tosend);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			BufferedReader br;
			try {
				br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
				String read = br.readLine();
				Log.d("MY", "Socket Read: " + read);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}			
		}
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
	
}
