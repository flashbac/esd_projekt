package bht.esd;

import java.awt.Color;
import java.awt.Panel;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import javax.swing.JFrame;

public class UDPserver {

	public static void main(String[] args) throws IOException {
		
		JFrame frame = new JFrame("Display image");
		  Panel panel = new ImagePanel();
		  panel.setSize(688,488);
		  panel.setBackground(Color.CYAN);
		  frame.getContentPane().add(panel);
		  frame.setSize(650, 490);
		  frame.setVisible(true);
		
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
        p.setPanel((ImagePanel)panel);
        
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