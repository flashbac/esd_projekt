package bht.ti.facefinder;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;


import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

public class ClientController {
	
	private String IP;
	private int Port;
	private Socket socket;
	
	private ReciveData reciver;
	private Thread reciverThread;
	
	private SendData sender;
	private Thread senderThread;
	
	public Handler sendHandler;
	public Handler reciveHandler;
	
	private PrintWriter out;
	private KontrollProtokoll kontrollProtokoll;
	
	public ClientController(String ip, int Port) {
		setIP(ip);
		setPort(Port);
		
		reciveHandler = new Handler()
		{
			@Override
			public void handleMessage(Message msg) {
		    	
				String read = (String)msg.obj;
		    	Log.d("MY", "Heander Read: " + read);
		    	// do something with read
		    	kontrollProtokoll.VerarbeiteAnfrage(read);	
			}
		};	
	}

	public void sendKommando(String jsonString) {
		Message msg = Message.obtain();
		msg.obj = jsonString; // Some Arbitrary object
		sendHandler.sendMessage(msg);
	}

	public int connect() {
		// client = new ClientThread();
		if (socket != null)
		{
			try {
				socket.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		reciver = new ReciveData();
		sender = new SendData();
		senderThread = new Thread (sender);
		reciverThread = new Thread(reciver);
		
		ConnectServer cs = new ConnectServer();
		Thread ts = new Thread (cs);
		ts.start();
		try {
			ts.join();
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

		if (socket == null)
		{
			return -1;
		}
		return 0;
	}

	public void disconnect() {
		
		if (socket != null)
		{
			sendKommando("exit");
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			try {
				socket.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		senderThread = null;
		reciverThread = null;
		
		socket = null;
	}
	
	class ConnectServer extends Thread {
		
		@Override
		public void run() {
			try {
				socket = new Socket(getIP(), getPort());
				
				try {
					out = new PrintWriter(socket.getOutputStream(), true);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				senderThread.start();
				reciverThread.start();
				
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}
		
	}
	
	class SendData extends Thread {
		@Override
		public void run() {
		
			Looper.prepare();
			sendHandler = new Handler() {
				public void handleMessage(Message msg) {
	                    
					String tosend = (String)msg.obj;
        	   
					if (socket != null)
					{   
						Log.d("MY", "Socket Send: " + tosend);
						out.println(tosend + ";");
					}    	   
				}   
			};
	        
			Looper.loop();
		}
	}
	
	class ReciveData implements Runnable {
			
		public ReciveData() {

		}

		@Override
		public void run() {

			if (socket != null) {
				BufferedReader br = null;
				try {
					br = new BufferedReader(new InputStreamReader(
							socket.getInputStream()));
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				try {
					String read = "";
					int item;
					while (socket != null) {
						read = "";
						while ((item = br.read()) != -1 ) {
							System.out.print((char) item);
							read += (char) item;
							if (!br.ready())
							{
								break;
							}
						}
						if (read.compareTo("")!=0)
						{
							Message msg = Message.obtain();
							msg.obj = read; // Some Arbitrary object
							reciveHandler.sendMessage(msg);
							Log.d("MY", "Socket Read: " + read);
						}
					}
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}
	
	public String getIP() {
		return IP;
	}

	public void setIP(String iP) {
		IP = iP;
	}

	public int getPort() {
		return Port;
	}

	public void setPort(int port) {
		Port = port;
	}

	public KontrollProtokoll getKontrollProtokoll() {
		return kontrollProtokoll;
	}

	public void setKontrollProtokoll(KontrollProtokoll kontrollProtokoll) {
		this.kontrollProtokoll = kontrollProtokoll;
	}
	
}
