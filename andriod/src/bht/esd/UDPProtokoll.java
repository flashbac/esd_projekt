package bht.esd;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.widget.ImageView;

public class UDPProtokoll {
	private static final byte UDP_HEADER_VERSION_1 = 0x01;
	private static final int UDP_HEADER_VERSION_1_OFFSET = 6;

	LinkedList<UDPProtokollChunk> chunkList;
	LinkedList<UDPProtokollBlob> blobList;
	ImageView panel;
	
	public UDPProtokoll() {
		// TODO Auto-generated constructor stub
		chunkList = new LinkedList<UDPProtokollChunk>();
		blobList = new LinkedList<UDPProtokollBlob>();
	}

	public void receive(DatagramPacket receivePacket) {
		/*
		 * Protokoll-Version[1Byte]; Bildnummer[2Byte]; Paketnummer[1Byte];
		 * Kamera-ID[1Byte]; Paketanzahl[1Byte]; Bild-Payload[~1468Bytes]
		 */
		byte[] data = receivePacket.getData();
		byte[] data2 = null;

		if (receivePacket.getLength() < UDP_HEADER_VERSION_1_OFFSET + 1) {
			return;
		}

		switch (data[0]) {
		case UDP_HEADER_VERSION_1:
			data2 = Arrays.copyOfRange(data, UDP_HEADER_VERSION_1_OFFSET,
					receivePacket.getLength() + UDP_HEADER_VERSION_1_OFFSET);
			chunkList.add(new UDPProtokollChunk(data[0],
					((data[1] << 8) & 0xff00) + (data[2] & 0xff), data[3],
					data[4], data[5], data2));
			break;
		default:
			return;
		}
		checkForCompleteItems();
	}

	void clear() {
		chunkList.clear();
		chunkList = new LinkedList<UDPProtokollChunk>();
		blobList.clear();
		blobList = new LinkedList<UDPProtokollBlob>();
	}

	void checkForCompleteItems() {
		List<UDPProtokollChunk> result = new ArrayList<UDPProtokollChunk>();
		
		int blobSize = 0;
		
		for (UDPProtokollChunk chunk : chunkList) {
			if (chunk.bild_id == chunkList.getLast().bild_id) {
				result.add(chunk);
				blobSize += chunk.getData().length;
			}
		}
		
		//speicher reservieren
		byte[] data = new byte[blobSize];
		int aktuellePos = 0;
		
		ByteBuffer b = ByteBuffer.allocate(blobSize+1);
		
		if ((!result.isEmpty())
				&& (result.size() == result.get(0).getPaket_anzahl())) {
			Collections.sort(result);
			// alle daten zusammenfassen
			int tmp_bild_id = -1;
			for (UDPProtokollChunk chunk : result)
			{
				b.put(chunk.getData(), 0, chunk.getData().length-UDP_HEADER_VERSION_1_OFFSET);
				System.arraycopy(chunk.getData(), 0, data, aktuellePos, chunk.getData().length-UDP_HEADER_VERSION_1_OFFSET);
				aktuellePos += chunk.getData().length-UDP_HEADER_VERSION_1_OFFSET;
				tmp_bild_id = chunk.getBild_id();
			}
			chunkList.removeAll(result);
			//blobList.add(new UDPProtokollBlob(tmp_bild_id, data));
			
			/*
			 * 
			 * nicht schn was jetzt kommt
			 */
			/*
			File file = new File("/home/dennis/git/esd_projekt/tests/java_udp_test/" + "t"+tmp_bild_id+".jpg");
			b.rewind();
			try{
				FileOutputStream asd = new FileOutputStream(file, false);
				FileChannel wc = asd.getChannel();
				wc.write(b);
				wc.close();
				asd.close();
			}catch(IOException e){
				
			}
			*/
			b.rewind();
			Bitmap bmp;
			BitmapFactory.Options options = new BitmapFactory.Options();
			options.inMutable = true;
			bmp = BitmapFactory.decodeStream(new ByteArrayInputStream(b.array())); // decodeByteArray(data, 0, data.length, options);
			
			panel.setImageBitmap(bmp);
		}
	}

	public void setPanel(ImageView p) {
		panel = p;
	}
	
	public void printBlobs() {
		for (UDPProtokollBlob blob : blobList) {
			System.out.println("Blob: bild_id: " + blob.getId()
					+ " datalength: " + blob.getData().length);
		}
	}
}
