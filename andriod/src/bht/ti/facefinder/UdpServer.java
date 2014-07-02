package bht.ti.facefinder;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import android.os.Handler;
import android.util.Log;
import bht.esd.UDPProtokoll;

public class UdpServer implements Runnable{
	
	private Handler handler;

	private Boolean run = false;
	private DatagramSocket serverSocket = null;
	private int mtu = 1500;

	
	public UdpServer (Handler handler)
	{
		this.handler = handler;
	}
		
	public void Stop()
	{
		run = false;
		serverSocket.close();
	}
	
	public void run() 
	{
		run = true;
		// TODO Auto-generated method stub
		System.out.println("test");
		try {
			serverSocket = new DatagramSocket(50000);
			Log.i("MY", "Size: " + serverSocket.getReceiveBufferSize());
			Log.i("MY", "Timeout: " + serverSocket.getSoTimeout());
			
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

      byte[] receiveData = new byte[mtu];
      UDPProtokoll p = new UDPProtokoll();
      //p.setPanel(image);
      p.setHeandler(handler);
      while(run)
         {
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            try {
				serverSocket.receive(receivePacket);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} 
		
		    // mit dem Bild was machen
		    p.receive(receivePacket);
		    p.printBlobs();
		    
		}
	}

	public int getMtu() {
		return mtu;
	}

	public void setMtu(int mtu) {
		this.mtu = mtu;
	}
}
