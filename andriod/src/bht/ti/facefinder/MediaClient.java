package bht.ti.facefinder;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.PipedReader;
import java.io.PipedWriter;
import java.nio.CharBuffer;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Handler;
import android.os.Message;
import android.util.Base64;
import android.util.Log;
import android.widget.ImageView;

public class MediaClient {
	
	private ImageView leimwand;
	
	 protected Handler handler = new Handler() {
		    @Override
		    public void handleMessage(Message msg) {
		    	Bitmap bm = (Bitmap)msg.obj;
		    	leimwand.setImageBitmap(bm);
		    }
		  };
	
	public MediaClient(ImageView l)
	{
		leimwand = l;
	}
	
	public void Start()
	{

		UdpServer udp;
		udp	= new UdpServer(handler);
		Thread udpthread = new Thread(udp);
		udpthread.start();
	}
		
}
