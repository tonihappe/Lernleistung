package animator;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;

public class Animated1D {

	private final float circleRadWidth = 50f;
	private final float posYdia = 0.75f;
	
	private float xmax, xmin;
	private ACanvas draw;
	private volatile boolean running = false;
	private int offset;
	private BufferedImage dia;
	protected Table table;
	protected int row;
	private Thread aniThread;
	
	public Animated1D(int width, int height, float xmin, float xmax) {
		this.xmax = xmax;
		this.xmin = xmin;
		
		draw = new ACanvas(width, height);
		offset = width / 20;
		
		dia = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
		drawDia();
		
//		aniThread = new Thread(draw);
		draw.set();
	}
	
	public void setAnimationTable(Table t, int row) {
		this.table = t;
		this.row = row;
	}
	
	private void drawDia(){
		Graphics g = dia.createGraphics();
		
		drawRect(g);
		drawLine(g);
		drawNumber(g, xmax);
		drawNumber(g, xmax / 2);
		drawNumber(g, xmin);
		drawNumber(g, xmin / 2);
		drawNumber(g, 0f);
		
		g.dispose();
	}
	
	public Canvas getDrawCanvas() {
		return draw;
	}
	
	private void drawRect(Graphics g){
		g.setColor(Color.black);
		g.drawRect(0, 0, draw.getWidth() - 1, draw.getHeight() - 1);
	}
	
	private void drawLine(Graphics g){
		g.setColor(Color.black);
		g.drawLine(offset, (int) (draw.getHeight() * posYdia), draw.getWidth() - offset, (int) (draw.getHeight() * posYdia));
	}
	
	private void drawNumber(Graphics g, float num){
		String n = num + "";
		
		int pixel = (int) (((draw.getWidth() - 2 * offset) / (xmax - xmin)) * (num - xmin)) + offset;
		g.drawLine(pixel, (int) (draw.getHeight() * posYdia) + 2, pixel, (int) (draw.getHeight() * posYdia) - 2);
		
		System.out.println(num + ": " + pixel);
		
		g.drawString(n, pixel - g.getFontMetrics().stringWidth(n) / 2, (int) (draw.getHeight() * posYdia) + g.getFontMetrics().getHeight() + 1);
	}
	
	private int drawCircle(Graphics g, float num, Color color){
		int pixel = (int) (((draw.getWidth() - 2 * offset) / (xmax - xmin)) * (num - xmin)) + offset;

		g.setColor(color);
		g.fillOval((int) (pixel - draw.getWidth() / (circleRadWidth * 2f)), (int) (draw.getHeight() / 2 - draw.getWidth() / (circleRadWidth * 2f)), (int) (draw.getWidth() / circleRadWidth), (int) (draw.getWidth() / circleRadWidth));
		return pixel;
	}
	
	public void startAnimation(){
		if(running || table == null) return;
		aniThread = new Thread(draw);
		running = true;
		aniThread.start();
	}
	
	@SuppressWarnings("deprecation")
	public void stopAnimation(){
		if(!running) return;
		
		running = false;
		aniThread.stop();
	}
	
	public float getXMax() {
		return xmax;
	}
	
	class ACanvas extends Canvas implements Runnable, MouseListener {
		
		private static final long serialVersionUID = 1L;
		
		private BufferedImage img;
		private float f, fs = Float.NaN;
		private float f2, fs2 = Float.NaN;
		boolean drawed = false;
		
		public ACanvas(int width, int height) {
			setSize(width, height);
			
			img = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
			
			addMouseListener(this);
		}
		
		protected void set() {
			Graphics g = img.createGraphics();
			g.drawImage(dia, 0, 0, null);
			g.dispose();
		}
		
		private Color color = new Color(12, 222, 0, 100);
		
		@Override
		public void paint(Graphics g) {
			if(!drawed) {
				g.drawImage(img, 0, 0, null);
			}
			
			drawCircle(g, fs, draw.getBackground());
			drawCircle(g, fs2, draw.getBackground());
			drawCircle(g, f, color);
			drawCircle(g, f2, color);
//			g.drawImage(dia, 0, 0, draw.getWidth() - 1, draw.getHeight() - 1, null);
//			g.drawImage(dia, 0, 0, null);
		}

		@SuppressWarnings("static-access")
		public void run() {
//			Graphics g = img.createGraphics();
			
			for(int i = 0; i < table.graphes[row].length; i++) {
				//CLEAN
				if(i != 0) {
//					drawCircle(g, f, draw.getBackground()); 
				}
				
				//SEARCH NEXT NEW
				do {
					f = table.graphes[row][i];
					i++;
				}while(f == fs);
				if(fs == Float.NaN) fs = f;
				
				f2 = table.graphes[row + 1][i];
				if(fs2 == Float.NaN) fs2 = fs;
				
				try {
					aniThread.sleep(0);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				repaint();
				fs = f;
				fs2 = f2;
				drawed = !drawed && i % 2 == 0;
			}
			running = false;
			aniThread.interrupt();
//			g.dispose();
		}

		public void mouseClicked(MouseEvent e) {
			
		}

		public void mousePressed(MouseEvent e) {
			startAnimation();
		}

		public void mouseReleased(MouseEvent e) {
			
		}

		public void mouseEntered(MouseEvent e) {
			
		}

		public void mouseExited(MouseEvent e) {
			
		}
	}
}
