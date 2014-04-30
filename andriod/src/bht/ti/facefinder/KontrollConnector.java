package bht.ti.facefinder;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class KontrollConnector {
	
	private String IP;
	private int Port;
	
	
	public KontrollConnector(String ip, int Port) {
		// TODO Auto-generated constructor stub
		setIP(ip);
		setPort(Port);		
		
		ConnectServer();
	}

	private void ConnectServer()
	{
		Socket socket;
		try {
			socket = new Socket(getIP(),getPort());
			PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
			out.println("test");
			socket.close();
		
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
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
