package bht.esd;

public class UDPProtokollChunk implements Comparable<UDPProtokollChunk>{

	int version;
	int bild_id;
	int chunk_id;
	int kamera_id;
	int paket_anzahl;
	byte[] data;

	UDPProtokollChunk() {
		super();
	}

	/*
	 * Protokoll-Version[1Byte]; Bildnummer[2Byte]; Paketnummer[1Byte];
	 * Kamera-ID[1Byte]; Paketanzahl[1Byte]; Bild-Payload[~1468Bytes]
	 */
	UDPProtokollChunk(int version, int bild_id, int chunk_id, int kamera_id,
			int paket_anzahl, byte[] data) {
		this.version = version;
		this.bild_id = bild_id;
		this.chunk_id = chunk_id;
		this.kamera_id = kamera_id;
		this.paket_anzahl = paket_anzahl;
		this.data = data.clone();
	}

	public int getChunk_id() {
		return chunk_id;
	}

	public void setChunk_id(int chunk_id) {
		this.chunk_id = chunk_id;
	}

	public int getVersion() {
		return version;
	}

	public void setVersion(int version) {
		this.version = version;
	}

	public int getBild_id() {
		return bild_id;
	}

	public void setBild_id(int bild_id) {
		this.bild_id = bild_id;
	}

	public int getKamera_id() {
		return kamera_id;
	}

	public void setKamera_id(int kamera_id) {
		this.kamera_id = kamera_id;
	}

	public int getPaket_anzahl() {
		return paket_anzahl;
	}

	public void setPaket_anzahl(int paket_anzahl) {
		this.paket_anzahl = paket_anzahl;
	}

	public byte[] getData() {
		return data;
	}

	public void setData(byte[] data) {
		this.data = data.clone();
	}

	public void printInfos() {
		System.out.println("Version: " + version + " bildID: " + bild_id
				+ " chunk_id: " + chunk_id + " kameraID: " + kamera_id
				+ " Paketanzahl: " + paket_anzahl + " data_length: "
				+ data.length);
	}
	
	public int compareTo(UDPProtokollChunk compareObject) {
		if (getChunk_id() < compareObject.getChunk_id())
			return -1;
		else if (getChunk_id() == compareObject.getChunk_id())
			return 0;
		else
			return 1;
	}

}
