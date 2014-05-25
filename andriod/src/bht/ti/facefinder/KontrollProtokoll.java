package bht.ti.facefinder;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

public class KontrollProtokoll {
	
	private ClientController client;
	
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
			jsonobject.put("mode", mode.toString());
		} catch (JSONException e) {
			e.printStackTrace();
		}
		SendeKommando(jsonobject.toString());
	}
	
	public void Kamera(int kameraID)
	{
		JSONObject jsonobject = new JSONObject();
		try {
			jsonobject.put("camera", kameraID);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		SendeKommando(jsonobject.toString());
	}
	
	public void StreamAnfordern(String ip, int port)
	{
		JSONObject jsonobject = new JSONObject();
		try {
			jsonobject.put("des", ip);
			jsonobject.put("port", port);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		SendeKommando(jsonobject.toString());
	}
	
	public void Position(KameraPosition p, int Steps)
	{
		JSONObject jsonobject = new JSONObject();
		try {
			jsonobject.put("position", p.toString());
			jsonobject.put("steps", Steps);
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
			String cmd = jsonObject.getString("cmd");
			JSONObject value = jsonObject.getJSONObject("value");
			Log.i("MY","Value: " + jsonObject.getString("mode"));
		
		
		} catch (JSONException e) {
			e.printStackTrace();
		}
		
		
	}
}
