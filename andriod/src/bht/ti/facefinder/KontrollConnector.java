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
	private PipedWriter RecivePW;
	private PipedReader RecivePR;
	
	private ReciveData reciver;
	private Thread reciverThread;
	
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
		
		RecivePR = new PipedReader();
		RecivePW = new PipedWriter();
		try {
			RecivePW.connect(RecivePR);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		try {
			socket = new Socket(getIP(),getPort());
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		reciver = new ReciveData();
		runningThread = new Thread(this);
		reciverThread = new Thread(reciver);
		runningThread.start();
		reciverThread.start();
		
		
	}	
	
	public void disconnect() {
		try {
			socket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		socket = null;
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
	
	
	class ReciveData implements Runnable
	{

		public ReciveData(){
			
				
		}
		
		@Override
		public void run() {
			
			BufferedReader br = null;
			try {
				br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			
			while(socket != null)
			{
				try {		
					String read = br.readLine();
					RecivePW.write(read);
				Log.d("MY", "Socket Read: " + read);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		
	
	}
}
