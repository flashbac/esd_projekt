package bht.ti.facefinder;

import android.app.Activity;
import android.app.Fragment;
import android.app.ProgressDialog;
import android.graphics.PixelFormat;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;
import android.net.Uri;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.MediaController;
import android.widget.VideoView;

public class ViewActivity extends Activity {
	
	public KontrollConnector verbindung;
	private String videoPath ="https://archive.org/download/Pbtestfilemp4videotestmp4/video_test.mp4";

	 private static ProgressDialog progressDialog;
	 private String videourl;  
	 private VideoView videoView;
	 

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_view);

		if (savedInstanceState == null) {
			getFragmentManager().beginTransaction()
					.add(R.id.container, new PlaceholderFragment()).commit();
		}
		
		Bundle b = getIntent().getExtras();
		int port = b.getInt("port");
		String ip = b.getString("ip");
		videoView = (VideoView) findViewById(R.id.videoView1);
		verbindung = new KontrollConnector(ip, port);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.view, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		if(id == R.id.kamera)
		{
			
			progressDialog = ProgressDialog.show(ViewActivity.this, "", "Buffering video...", true);
			progressDialog.setCancelable(true);  


			PlayVideo();
			return true;
		}
		if (id == R.id.auto_mode) {
			verbindung.sendKommando("[commando:automode];");
			return true;
		}
		if (id == R.id.manual_mode) {
			verbindung.sendKommando("[commando:manualmode];");
			return true;
		}
		if (id == R.id.settingBack) {
			setContentView(R.layout.activity_main);
			verbindung.sendKommando("exit");
			return true;
		}
		if (id == R.id.connect) {
			//verbindung = new KontrollConnector(ip, Integer.parseInt(port));
			return true;
		}
		
		
		return super.onOptionsItemSelected(item);
	}

	/**
	 * A placeholder fragment containing a simple view.
	 */
	public static class PlaceholderFragment extends Fragment {

		public PlaceholderFragment() {
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container,
				Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_view, container,
					false);
			return rootView;
		}
	}
	

	 private void PlayVideo()
	 {
	  try
	       {      
	              getWindow().setFormat(PixelFormat.TRANSLUCENT);
	              MediaController mediaController = new MediaController(ViewActivity.this);
	              mediaController.setAnchorView(videoView);           

	               Uri video = Uri.parse(videoPath );             
	               videoView.setMediaController(mediaController);
	               videoView.setVideoURI(video);
	               videoView.requestFocus();              
	               videoView.setOnPreparedListener(new OnPreparedListener()
	               {

	                   public void onPrepared(MediaPlayer mp)
	                   {                  
	                       progressDialog.dismiss();     
	                       videoView.start();
	                   }
	               });           


	            }
	       catch(Exception e)
	       {
	                progressDialog.dismiss();
	                System.out.println("Video Play Error :"+e.toString());
	                finish();
	       }   

	 }
}
