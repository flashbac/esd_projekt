package bht.ti.facefinder;

import android.app.Activity;
import android.app.Fragment;
import android.app.ProgressDialog;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;
import android.net.Uri;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.MediaController;
import android.widget.Toast;
import android.widget.VideoView;

public class ViewActivity extends Activity {
	
	public ClientController verbindung = null;
	public KontrollProtokoll kontrollProtokoll;

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
		verbindung = new ClientController(ip, port);
		kontrollProtokoll = new KontrollProtokoll(verbindung);
		
		
		if (verbindung.connect() == -1)
		{
			Toast.makeText(ViewActivity.this, "Can not Connect to " + verbindung.getIP() + ".", Toast.LENGTH_SHORT).show();
			Intent intent = new Intent(ViewActivity.this, MainActivity.class);
		    startActivity(intent);
		    finish();
		}
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
		if(id == R.id.startStream)
		{
			kontrollProtokoll.StreamAnfordern(Helper.getLocalIpAddress(), 50000);
			return true;
		}
		if (id == R.id.auto_mode) {
			kontrollProtokoll.Mode(KameraMode.Auto);
			return true;
		}
		if (id == R.id.manual_mode) {
			kontrollProtokoll.Mode(KameraMode.Manual);
			return true;
		}
		if (id == R.id.trennen) {
						
			kontrollProtokoll.TrenneVerbindung();
			Intent intent = new Intent(ViewActivity.this, MainActivity.class);
			Bundle b = new Bundle();
		    b.putBoolean("exit", true);
		    startActivity(intent);
		    finish();
			
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
		 
	 @Override
	 public boolean onKeyDown(int keyCode, KeyEvent event)
	 {
	     if ((keyCode == KeyEvent.KEYCODE_BACK))
	     {
	         finish();
	     }
	     return super.onKeyDown(keyCode, event);
	 }
	 	 
	 @Override
	 public void finish() {
		// TODO Auto-generated method stub
		verbindung.disconnect();
		super.finish();
	}
	 
}
