package bht.ti.facefinder;

import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.JsonReader;
import android.util.Log;

public class KontrollProtokoll {
	
	private ClientController client;
	public ArrayList<Kamera> kameras = new ArrayList<Kamera>();
	public ArrayList<Face> faces = new ArrayList<Face>();
	
	public KontrollProtokoll(ClientController c)
	{
		client = c;
		client.setKontrollProtokoll(this);
	}
	
	private void SendeKommando(String json)
	{	
		client.sendKommando(json);
	}
		
	public void Mode(KameraMode mode)
	{
		JSONObject jsonobject = new JSONObject();
		try {
			jsonobject.put("cmd", "mode");
			jsonobject.put("value", mode.toString());
		} catch (JSONException e) {
			e.printStackTrace();
		}
		SendeKommando(jsonobject.toString());
	}
	
	public void Kamera(int kameraID)
	{
		JSONObject jsonobject = new JSONObject();
		try {
			jsonobject.put("cmd", "camera");
			jsonobject.put("value", kameraID);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		SendeKommando(jsonobject.toString());
	}

	public void Face(int faceID)
	{
		JSONObject jsonobject = new JSONObject();
		try {
			jsonobject.put("cmd", "fase");
			jsonobject.put("value", faceID);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		SendeKommando(jsonobject.toString());
	}
	
	public void StreamAnfordern(String ip, int port)
	{
		JSONObject jsonobject = new JSONObject();
		JSONObject innerob = new JSONObject();
		try {
			jsonobject.put("cmd", "udp");
			innerob.put("des", ip);
			innerob.put("port", port);
			jsonobject.put("value", innerob);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		SendeKommando(jsonobject.toString());
	}
	
	public void Position(KameraPosition p, int Steps)
	{
		JSONObject jsonobject = new JSONObject();
		JSONObject innerob = new JSONObject();
		try {
			jsonobject.put("cmd", "position");
			innerob.put("direction", p.toString());
			innerob.put("steps", Steps);
			jsonobject.put("value", innerob);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		SendeKommando(jsonobject.toString());
	}
	
	public void TrenneVerbindung()
	{
		JSONObject jsonobject = new JSONObject();
		try {
			jsonobject.put("cmd", "exit");
			jsonobject.put("value", 1);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		SendeKommando(jsonobject.toString());
	}
	
	public void VerarbeiteAnfrage(String json)
	{
		try {
			JSONObject jsonObject = new JSONObject(json);
			
			if (jsonObject.has("status"))
			{
				String cmd = jsonObject.getString("status");
				
				switch (TcpProtokollStatus.valueOf(cmd))
				{
					case cams: 
					{
						JSONArray innerArray = jsonObject.getJSONArray("value");
						
						kameras.clear();
						for (int i = 0; i< innerArray.length();i++ )
						{
							JSONObject innerOb = innerArray.getJSONObject(i);
							kameras.add(new Kamera(innerOb.getInt("id"),
									innerOb.getString("name"),
									innerOb.getBoolean("use")));	
						}
						
						
						break;
					}
					case camera:
					{
						int value = jsonObject.getInt("value");
						
						Log.i("TCPP","Current Camera is Camera "+ value + "." );
						break;
					}
					case udp:
					{
						JSONObject innerOb = jsonObject.getJSONObject("value");
						String des = innerOb.getString("des");
						int port = innerOb.getInt("port");
						Log.i("TCPP","Udp package sent to " + des + ":" + port );
						break;
					}
					case face:
					{
						JSONArray innerArray = jsonObject.getJSONArray("value");
					
						//ArrayList<Face> faces = new ArrayList<Face>();
						faces.clear();
						for (int i = 0; i< innerArray.length();i++ )
						{
							JSONObject innerOb = innerArray.getJSONObject(i);
							faces.add(new Face(innerOb.getInt("id"),
									innerOb.getString("name"),
									innerOb.getInt("x"),
									innerOb.getInt("y"),
									innerOb.getInt("width"),
									innerOb.getInt("height")));
						}
						Log.i("TCPP","Empfange " + faces.size() + " Faces.");
						break;
					}
					case track:
					{
						int value = jsonObject.getInt("value");
						Log.i("TCPP","Track Camera with ID " + value );
						break;
					}
					case position:
					{
						JSONObject value = jsonObject.getJSONObject("value");
						int x = value.getInt("x");
						int y = value.getInt("y");
						Log.i("TCPP","Camera Position x: " + x + " y:" + y );
						break;
					}
					case mtu:
					{
						int value = jsonObject.getInt("value");
						Log.i("TCPP","MTU: " + value );
						
						
						break;
					}
					default:
				}
			}
				
			if (jsonObject.has("cmd"))
			{
				String cmd = jsonObject.getString("cmd");
				if ( cmd.endsWith("exit"))
				{
					JSONObject value = jsonObject.getJSONObject("value");
					Log.i("TCPP","Exit Client.");
					client.disconnect();
				}
			}
		} catch (JSONException e) {
			e.printStackTrace();
		}
		
		
	}
}
