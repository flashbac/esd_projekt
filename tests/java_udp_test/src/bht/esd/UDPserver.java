package bht.esd;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class UDPserver {

	public static void main(String[] args) throws IOException {
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
              System.out.println("from " +receivePacket.getAddress() +" RECEIVED: " + sentence);
              
              p.receive(receivePacket);
              p.printBlobs();
           }
	}

}