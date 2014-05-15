package bht.ti.facefinder;

import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.ImageView;

public class MediaClient {
	
	private ImageView leimwand;
	
	 protected Handler handler = new Handler() {
		    @Override
		    public void handleMessage(Message msg) {
		    	Bitmap bm = (Bitmap)msg.obj;
		    	leimwand.setImageBitmap(bm);
		    	
		    	Runtime info = Runtime.getRuntime();
		    	Log.i("MEM", "free: " + info.freeMemory() + " total: " +  info.totalMemory());
		    	//System.gc();
		    	//Log.i("MY", "free: " + info.freeMemory() + " total: " +  info.totalMemory());
		    	
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
