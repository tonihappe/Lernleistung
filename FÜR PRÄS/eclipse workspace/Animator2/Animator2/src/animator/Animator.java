package animator;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.UIManager;

public final class Animator {

	private JFrame frame;
	private JPanel contentPane;
	
	public Animator(int width, int height){
		createFrame(width, height);
		createPanel(width, height);
	}
	
	public void createPanel(int width, int height){
		contentPane = new JPanel();
		contentPane.setLayout(null);
	}
	
	protected void createFrame(int width, int height){
		frame = new JFrame("Animator");
		frame.setSize(width, height);
		frame.setResizable(false);
		frame.setLocationRelativeTo(null);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLayout(null);
		
		try{
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public JFrame getFrame() {
		return frame;
	}
	
	public int getWidth(){
		return frame.getWidth();
	}
	
	public int getHeight(){
		return frame.getHeight();
	}
	
	public void addDiagram(Diagram dia, int x, int y){
		dia.setVisible(true);
		dia.setLocation(x, y);
		
		contentPane.add(dia);
	}
	
	public void addDiagram2D(Diagramm2D dia, int x, int y){
		dia.setVisible(true);
		dia.setLocation(x, y);
		
		contentPane.add(dia);
	}
	
	public void addAnimated1D(Animated1D animated, int x, int y){
		animated.getDrawCanvas().setVisible(true);
		animated.getDrawCanvas().setLocation(x, y);
	
		contentPane.add(animated.getDrawCanvas());
	}
	
	public void build(){
		frame.setContentPane(contentPane);
		frame.setVisible(true);
	}
}
