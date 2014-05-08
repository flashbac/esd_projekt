package bht.ti.facefinder;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import android.util.Log;

public class Helper {
	
	/*
	 * Nicht 100 % implementiert !!!
	 * 
	 */
	public static String getLocalIpAddress() {
	    try {
	    	Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces();
	    	while ( en.hasMoreElements())
	    	{
	    		NetworkInterface intf = en.nextElement();
	    		String name =intf.getName();
	            if (name.compareTo("eth0") == 0 || name.compareTo("wlan0") == 0)
	            {
	            	Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses();
	            	while (enumIpAddr.hasMoreElements())
	            	{
	            		InetAddress inetAddress = enumIpAddr.nextElement();
	            		if (inetAddress.getClass() == Inet4Address.class)
	            		{
	            			return inetAddress.getHostAddress();
	            		}
	            	}
	            }
	    	}
	    } catch (SocketException ex) {
			Log.e("MY", ex.toString());
	    }
	    return null;
	}
}
