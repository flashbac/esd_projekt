package bht.ti.facefinder;

import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.ImageView;

public class MediaClient {
	
	private ImageView leimwand;
	private UdpServer udp = null;
	private Thread udpthread = null;
	
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
		if (udp == null)
		{
			udp	= new UdpServer(handler);
			udpthread = new Thread(udp);
			udpthread.start();
		}
		
	}
	
	public void Stop()
	{
		udp.Stop();
	}
		
}
