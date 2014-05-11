package bht.ti.facefinder;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketException;

import android.widget.ImageView;
import bht.esd.UDPProtokoll;

public class UdpServer implements Runnable{
	
	private ImageView image;
	
	public UdpServer (ImageView i)
	{
		image = i;
	}
		
	public void run()
	{
		
		// TODO Auto-generated method stub
		System.out.println("test");
		DatagramSocket serverSocket = null;
		try {
			serverSocket = new DatagramSocket(50000);
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
      byte[] receiveData = new byte[1500];
      UDPProtokoll p = new UDPProtokoll();
      p.setPanel(image);            
      while(true)
         {
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            try {
				serverSocket.receive(receivePacket);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
            String sentence = new String( receivePacket.getData());
            //System.out.println("from " +receivePacket.getAddress() +" RECEIVED: " + sentence);
            
            
            // mit dem Bild was machen
            p.receive(receivePacket);
            p.printBlobs();
            
         }
	}
}
