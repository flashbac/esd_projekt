package bht.ti.facefinder;

import android.app.Activity;
import android.webkit.WebView.FindListener;
import android.widget.VideoView;

public class MediaClient extends Activity implements Runnable {
	
	protected VideoView view;
	
	public void MediaClient(int Port, VideoView view)
	{
		this.view = view;
	}

	@Override
	public void run() {

		
		
		
	}
	
}
