package bht.ti.facefinder;

import java.util.ArrayList;

import android.app.Activity;
import android.app.Fragment;

import android.content.Intent;

import android.os.Bundle;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SubMenu;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ImageView;

import android.widget.Toast;


public class ViewActivity extends Activity implements OnClickListener {
	
	public ClientController verbindung = null;
	public KontrollProtokoll kontrollProtokoll;
	public MediaClient mediaClient = null;
	private static final int KAMERA_MENU = 10;
	private int mtu = 1500;

	
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
		
		initControllButtons();
		
	}

	private void initControllButtons() {
		ImageView buttonLeft = (ImageView) findViewById(R.id.buttonLeft);
		buttonLeft.setOnClickListener(this);
		
		ImageView buttonRight = (ImageView) findViewById(R.id.buttonRight);
		buttonRight.setOnClickListener(this);
		
		ImageView buttonBottom = (ImageView) findViewById(R.id.buttonBottom);
		buttonBottom.setOnClickListener(this);
		
		ImageView buttonTop = (ImageView) findViewById(R.id.buttonTop);
		buttonTop.setOnClickListener(this);
	}


	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		int group =  item.getGroupId();
		if (group == KAMERA_MENU)
		{
			kontrollProtokoll.Kamera(item.getItemId()); // TODO GEF�HRLICH
		}
		
		if (id == R.id.action_settings) {
			return true;
		}
		if(id == R.id.startStream)
		{
			if (mediaClient == null)
			{
				mediaClient = new MediaClient((ImageView)findViewById(R.id.streamingImage));
				mediaClient.Start();
			}
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

	@Override
	public void onClick(View v) {
		switch ( v.getId())
		{
			case R.id.buttonTop:
				kontrollProtokoll.Position(KameraPosition.Top, 1);
				break;
			case R.id.buttonBottom:
				kontrollProtokoll.Position(KameraPosition.Bottom, 1);
				break;
			case R.id.buttonLeft:
				kontrollProtokoll.Position(KameraPosition.Left, 1);
				break;
			case R.id.buttonRight:
				kontrollProtokoll.Position(KameraPosition.Right, 1);
				break;
		}
	}
	 
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.view, menu);
		return true;
	}
	
	@Override
	public boolean onPrepareOptionsMenu(Menu menu) {
		//menu.clear();
		MenuItem kameraMenu = (MenuItem)menu.findItem(R.id.kamera);
		if (kameraMenu != null && kontrollProtokoll.kameras.size()>0)
		{
			kameraMenu.getSubMenu().clear();
			SubMenu sm =  kameraMenu.getSubMenu();
			for (Kamera k : kontrollProtokoll.kameras) {
				sm.add(KAMERA_MENU, k.id, k.id, k.name);
				
			}
		}		
		return super.onPrepareOptionsMenu(menu);
	}
}
