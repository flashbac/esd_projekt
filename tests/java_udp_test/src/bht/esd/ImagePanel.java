package bht.esd;

import java.awt.Graphics;
import java.awt.Panel;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

import javax.imageio.ImageIO;

public class ImagePanel extends Panel {
	BufferedImage image;

	public ImagePanel() {
	}

	public void paint(Graphics g) {
		//super.paint(g);
		g.drawImage(image, 0, 0, null);
	}

	public void showPic(UDPProtokollBlob blob) {
		InputStream is = new ByteArrayInputStream(blob.getData());
		try {
			image = ImageIO.read(is);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		this.paint(this.getGraphics());
	}
	
	public void showPicByteBuffer(ByteBuffer buffer) {
		InputStream is = new ByteArrayInputStream(buffer.array());
		try {
			image = ImageIO.read(is);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		this.paint(this.getGraphics());
	}
}
