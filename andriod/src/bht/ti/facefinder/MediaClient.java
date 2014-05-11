package bht.ti.facefinder;

import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.webkit.WebView.FindListener;
import android.widget.ImageView;
import android.widget.VideoView;

public class MediaClient extends Activity implements Runnable {
	
	protected ImageView view;
	ImageView bmImage;
	
	public void MediaClient(int Port, ImageView view)
	{
		this.view = (ImageView)findViewById(R.id.imageStream2);
				
		this.bmImage = bmImage;
	}
	
	protected Bitmap doInBackground(String... urls) {
	    String urldisplay = urls[0];
	    Bitmap mIcon11 = null;
	    try {
	        InputStream in = new java.net.URL(urldisplay).openStream();
	        mIcon11 = BitmapFactory.decodeStream(in);
	    } catch (Exception e) {
	        Log.e("Error", e.getMessage());
	        e.printStackTrace();
	    }
	    return mIcon11;
	}

	protected void onPostExecute(Bitmap result) {
	    bmImage.setImageBitmap(result);
	}
	
	@Override
	public void run() {


		
		
	}
	
}
