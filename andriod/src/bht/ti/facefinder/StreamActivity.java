package bht.ti.facefinder;

import bht.ti.facefinder.MainActivity.PlaceholderFragment;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;

public class StreamActivity extends Activity {
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.steam_view);

		if (savedInstanceState == null) {
			getFragmentManager().beginTransaction()
					.add(R.id.container, new PlaceholderFragment()).commit();
		}
		
		Log.i("Programm", "StreamActivity is Active");
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.stream, menu);
		return true;
	}
}
