package animator;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.image.BufferedImage;

import javax.swing.JPanel;

public final class Diagram extends JPanel {

	private static final long serialVersionUID = 1L;
	
	private BufferedImage dia_raw;
	private BufferedImage graph;
	private int offsetX;
	private int offsetY;
	private float xmax, ymax;

	public Diagram(int width, int height, float xmax, float ymax) {
		this.xmax = xmax;
		this.ymax = ymax;
		
		setSize(width, height);
		
		//
		createDia(width, height);
		createGraph(width, height);
	}
	
	protected void createDia(int width, int height){
		dia_raw = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
		
		offsetX = dia_raw.getWidth() / 20;
		offsetY = dia_raw.getHeight() / 20;
		
		Graphics g = dia_raw.createGraphics();
		g.setColor(Color.black);
		g.drawRect(0, 0, dia_raw.getWidth() - 1, dia_raw.getHeight()- 1);
		
		//Y-ACHSE
		g.drawLine(offsetX, dia_raw.getHeight() - offsetY, offsetX, offsetY);
//		g.drawString("" + ymax, offsetX / 2 - g.getFontMetrics().stringWidth(ymax + ""), offsetY - 2);
		drawNumberY(ymax + "", g, ymax);
		//X-ACHSE
		g.drawLine(offsetX, dia_raw.getHeight() - offsetY, dia_raw.getWidth() - offsetX, dia_raw.getHeight() - offsetY);
		g.drawString("" + xmax, dia_raw.getWidth() - offsetX + 2, dia_raw.getHeight() - offsetY);
		
		g.dispose();
	}
	
	public void drawNumberY(String str, float y_){
		Graphics g = dia_raw.createGraphics();
		drawNumberY(str, g, y_);
		g.dispose();
	}
	
	public void drawNumberY(String str, Graphics g, float y_){
		int y = dia_raw.getHeight() - (int) ((dia_raw.getHeight() - offsetY * 2) / ymax * y_) - offsetY;
		
		//Marker
		g.setColor(Color.BLACK);
		g.drawLine(offsetX - 2, y, offsetX + 2, y);
		
		if(str.length() > 7) str = str.substring(0, 7);
		
		//String
		int strWidth = g.getFontMetrics().stringWidth(str);
		g.drawString(str, offsetX - strWidth - 2, y + g.getFontMetrics().getHeight() / 2);
	}
	
	protected void createGraph(int width, int height){
		graph = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
	}
	
	public void drawGraph(float[] x_, float[] y_){
		drawGraph(x_, y_, Color.blue);
	}
	
	public void drawGraph(float[] x_, float[] y_, Color color){
		Graphics g = graph.createGraphics();
		g.setColor(color);
		int pixelXMax = graph.getWidth() - offsetX * 2;
		int pixelYMax = graph.getHeight() - offsetY * 2;
		
		int zx = 0, zy = 0;
		int x, y;
		
		for(int i = 0; i < x_.length; i++){
			x = (int) (pixelXMax / xmax * x_[i]) + offsetX;
			y = graph.getHeight() - (int) (pixelYMax / ymax * y_[i]) - offsetY;
			
			if(i > 0){
				g.drawLine(zx, zy, x, y);
			}
			
			zx = x;
			zy = y;
		}
		
		g.dispose();
	}
	
	public void drawTable(Table table, int... graphes){
		for(int i = 0; i < graphes.length; i+=2){
			drawGraph(table.graphes[graphes[i]], table.graphes[graphes[i + 1]], Color.blue);
		}
	}
	
	public void drawTable(Table table, Color[] colors, int... graphes){
		for(int i = 0; i < graphes.length; i+=2){
			drawGraph(table.graphes[graphes[i]], table.graphes[graphes[i + 1]], colors[i / 2]);
		}
	}
	
	public void paint(Graphics g) {
		super.paint(g);

		g.drawImage(dia_raw, 0, 0, getWidth(), getHeight(), null);
		g.drawImage(graph, 0, 0, getWidth(), getHeight(), null);
	}
}
