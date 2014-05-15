package bht.ti.facefinder;

import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;
import android.os.Build;

public class MainActivity extends Activity {

	public MediaClient mediaClient = null;
	
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
		btnIPAutoFill();
		
	}


	private void btnIPAutoFill() {
		// TODO Auto-generated method stub
		Button rensky = (Button)findViewById(R.id.rensky_net);
		rensky.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				EditText etip = (EditText) findViewById(R.id.editTextIpAddress);
				EditText etport = (EditText) findViewById(R.id.editTextPort);
				etip.getText().clear();
				etport.getText().clear();
				etip.getText().append("88.198.13.243");
				etport.getText().append("5000");
			}
		});
		
		Button home = (Button)findViewById(R.id.home_server);
		home.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				EditText etip = (EditText) findViewById(R.id.editTextIpAddress);
				EditText etport = (EditText) findViewById(R.id.editTextPort);
				etip.getText().clear();
				etport.getText().clear();
				etip.getText().append("192.168.1.40");
				etport.getText().append("5000");
			}
		});
	}


	private void btnConnectSetup() {
		// TODO Auto-generated method stub
		
		Button b = (Button)findViewById(R.id.btnConnect);
		EditText etip = (EditText) findViewById(R.id.editTextIpAddress);
		EditText etport = (EditText) findViewById(R.id.editTextPort);
		etip.getText().append("192.168.1.40");
		etport.getText().append("5000");
		
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
				
			    Intent intent = new Intent(MainActivity.this, ViewActivity.class);
			    Bundle b = new Bundle();
			    b.putInt("port", Integer.parseInt(port)); //Your id
			    b.putString("ip", ip);
			    intent.putExtras(b); //Put your id to your next Intent
			    startActivity(intent);
			    finish();
			    
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
		if (id == R.id.ipInfo) {
			
			String ip = Helper.getLocalIpAddress();
			Toast.makeText(MainActivity.this, "You IP is: " + ip + "", Toast.LENGTH_SHORT).show();
			return true;
		}
		if (id == R.id.startUdp) {
			if (mediaClient == null)
			{
				mediaClient = new MediaClient((ImageView)findViewById(R.id.imageStream2));
				mediaClient.Start();
			}
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
