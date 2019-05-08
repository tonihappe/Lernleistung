package animator;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.image.BufferedImage;

import javax.swing.JPanel;

public class Diagramm2D extends JPanel {

	private static final long serialVersionUID = 1L;
	
	private float xmin, ymin, xmax, ymax;
	private int offsetX, offsetY;
	
	private BufferedImage dia;
	private BufferedImage kurve;
	
	private int nullX, nullY;
	
	public Diagramm2D(int width, int height, float xmin, float ymin, float xmax, float ymax) {
		setSize(width, height);
		offsetX = width / 20;
		offsetY = height / 20;
		
		this.xmin = xmin;
		this.ymin = ymin;
		this.xmax = xmax;
		this.ymax = ymax;
		
		createDia(width, height);
	}
	
	private int getPixelX(float num){
		return (int) (((dia.getWidth() - 2 * offsetX) / (xmax - xmin)) * (num - xmin)) + offsetX;
	}
	
	private int getPixelY(float num){
		return dia.getHeight() - (int) (((dia.getHeight() - 2 * offsetY) / (ymax - ymin)) * (num - ymin)) + offsetY;
	}
	
	private void createDia(int width, int height){
		dia = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
		
		Graphics g = dia.createGraphics();
		
		g.setColor(Color.black);
		g.drawRect(0, 0, getWidth() - 1, getHeight() - 1);
		
		g.setColor(Color.black);
		g.drawLine(offsetX, nullY = getPixelY(0), width - offsetX, nullY);
		g.drawLine(nullX = getPixelX(0), offsetY, nullX, height - offsetY);
		
		System.out.println(nullY + ": " + (getHeight() / 2) + ": " + ymin + " " + ymax);
	}
	
	@Override
	public void paint(Graphics g) {
		super.paint(g);
		
		g.drawImage(dia, 0, 0, getWidth(), getHeight(), null);
	}
}
