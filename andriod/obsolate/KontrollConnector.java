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

import android.os.Looper;
import android.util.Log;
import android.widget.Toast;

public class KontrollConnector {

	private String IP;
	private int Port;
	private Socket socket;
	// private ClientThread client;
	private PipedWriter pw;
	private PipedReader pr;
	private PipedWriter RecivePW;
	private PipedReader RecivePR;

	private ReciveData reciver;
	private Thread reciverThread;
	
	private SendData sender;
	private Thread senderThread;

	public KontrollConnector(String ip, int Port) {
		// TODO Auto-generated constructor stub
		setIP(ip);
		setPort(Port);
		//connect();
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

		reciver = new ReciveData();
		sender = new SendData();
		senderThread = new Thread (sender);
		reciverThread = new Thread(reciver);
		senderThread.start();
		
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
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

	public void sendKommando(String jsonString) {

		try {
			pw.write(jsonString);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	class SendData implements Runnable {

		
		@Override
		public void run() {

			PrintWriter out = null;

			try {
				socket = new Socket(getIP(), getPort());
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				
			}
			if (socket != null)
			{
				reciverThread.start();
	
				try {
					out = new PrintWriter(socket.getOutputStream(), true);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
	
				int item;
				String tosend;
				while (socket != null) {
					try {
						tosend = "";
						while ((item = pr.read()) != -1 ) {
							System.out.print((char) item);
							tosend += (char) item;
							
							if (!pr.ready())
							{
								break;
							}
						}
						out.println(tosend);
						Log.d("MY", "Socket Send: " + tosend);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
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
							RecivePW.write(read);
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
