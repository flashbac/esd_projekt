package bht.ti.facefinder;

import java.io.IOException;
import java.io.PipedReader;
import java.io.PipedWriter;
import java.nio.CharBuffer;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Base64;
import android.util.Log;
import android.widget.ImageView;

public class MediaClient {
	
	private ImageView leimwand;
	private PipedWriter pw;
	private PipedReader pr;  
	
	public MediaClient(ImageView l)
	{
		leimwand = l;
	}
	
	public void Start()
	{
		pr = new PipedReader();
		pw = new PipedWriter();
		try {
			pw.connect(pr);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		UdpServer udp;
		udp	= new UdpServer(pw);
		Thread udpthread = new Thread(udp);
		udpthread.start();
	}
	
	public void DrawNewImage()
	{
		try{
			int item;
			String tosend = "";
			while ((item = pr.read()) != -1 ) {
				System.out.print((char) item);
				tosend += (char) item;
				
				//Endezeichen abfragen
				if (item == EOF)
				{
					break;
				}
				
				if (!pr.ready())
				{
					break;
				}
			}
			byte[] imageAsBytes = tosend.getBytes();
			Bitmap bp = BitmapFactory.decodeByteArray(imageAsBytes, 0, imageAsBytes.length);
			leimwand.setImageBitmap(bp);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
}
