package bht.esd;

import java.net.DatagramPacket;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

public class UDPProtokoll {
	private static final byte UDP_HEADER_VERSION_1 = 0x01;
	private static final int UDP_HEADER_VERSION_1_OFFSET = 6;

	LinkedList<UDPProtokollChunk> chunkList;
	LinkedList<UDPProtokollBlob> blobList;
	ImagePanel panel;

	public UDPProtokoll() {
		// TODO Auto-generated constructor stub
		chunkList = new LinkedList<UDPProtokollChunk>();
		blobList = new LinkedList<UDPProtokollBlob>();
		panel = null;
	}

	void receive(DatagramPacket receivePacket) {
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
		
		ByteBuffer b = ByteBuffer.allocate(blobSize);
		
		if ((!result.isEmpty())
				&& (result.size() == result.get(0).getPaket_anzahl())) {
			Collections.sort(result);
			// alle daten zusammenfassen
			int tmp_bild_id = -1;
			for (UDPProtokollChunk chunk : chunkList)
			{
				b.put(chunk.getData(), 0, chunk.getData().length);
				System.arraycopy(chunk.getData(), 0, data, aktuellePos, chunk.getData().length);
				aktuellePos += chunk.getData().length;
				tmp_bild_id = chunk.getBild_id();
			}
			chunkList.removeAll(result);
			//blobList.add(new UDPProtokollBlob(tmp_bild_id, data));
			
			/*
			 * 
			 * nicht schön was jetzt kommt
			 */
			panel.showPic(new UDPProtokollBlob(tmp_bild_id, data));
		}
	}
	
	public void setPanel(ImagePanel p)
	{
		panel = p;
	}
	
	void printBlobs()
	{
		for (UDPProtokollBlob blob : blobList) {
			System.out.println("Blob: bild_id: "+blob.getId()+" datalength: "+blob.getData().length);
		}
	}
}
