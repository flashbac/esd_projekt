package bht.ti.facefinder;

import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.os.Build;

public class MainActivity extends Activity {

	public KontrollConnector verbindung;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		if (savedInstanceState == null) {
			getFragmentManager().beginTransaction()
					.add(R.id.container, new PlaceholderFragment()).commit();
		}
		Log.i("Programm", "Start Programm");
		btnConnectSetup();
		
	}


	private void btnConnectSetup() {
		// TODO Auto-generated method stub
		
		Button b = (Button)findViewById(R.id.btnConnect);
		
		b.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				
				EditText etip = (EditText) findViewById(R.id.editTextIpAddress);
				EditText etport = (EditText) findViewById(R.id.editTextPort);
				String ip = etip.getText().toString();
				String port = etport.getText().toString();
				
				Log.i("BtnConnect", "IP:" + ip + " Port:" + port);
				Log.i("MY", "IP:" + ip + " Port:" + port);
				//verbindung = new KontrollConnector(ip, Integer.parseInt(port));
				setContentView(R.layout.steam_view);
			}
		});
		
	}


	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
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
		if (id == R.id.settingBack) {
			setContentView(R.layout.activity_main);
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
			View rootView = inflater.inflate(R.layout.fragment_main, container,
					false);
			return rootView;
		}
	}

}
