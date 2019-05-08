import java.awt.Color;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import animator.Animated1D;
import animator.Animator;
import animator.Diagram;
import animator.Table;


public class Starter {

	public static void main(String[] args) {
		Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
		
		Animator m = new Animator(d.width - 100, d.height - 100);
		long start = System.currentTimeMillis();
		Table t = Table.loadByFile(new File("Z:/SeagateNAS/Toni/Besondere Lernleistung/Projektordner/git/Lernleistung/Ausgabe.dat"),0,1);//byObject oder byFile
		System.out.println("Loaded Table: " + (System.currentTimeMillis() - start) + "ms");
		
		Diagram dia = new Diagram(m.getWidth() - 100, m.getHeight() / 2, t.graphes[0][t.graphes[0].length - 1], t.getMax(1));
		
		m.addDiagram(dia, 50, 50);
		//dia.drawTable(t, new Color[] {Color.red, Color.blue}, 0, 1, 0, 2);
		dia.drawGraph(t.graphes[0], t.graphes[1]);
		
		dia.drawNumberY(t.graphesMax[1] + "", t.graphesMax[1]);
		//dia.drawNumberY(t.graphesMax[2] + "", t.graphesMax[2]);
		
		dia.drawNumberY(t.graphesMin[1] + "", t.graphesMin[1]);
	//	dia.drawNumberY(t.graphesMin[2] + "", t.graphesMin[2]);
		
//		Animated1D ani = new Animated1D(m.getWidth() - 100, m.getHeight() / 2 - 150, t.getMin(1, 2), t.getMax(1, 2));
//		m.addAnimated1D(ani, 50, 100 + m.getHeight() / 2);
//		ani.setAnimationTable(t, 1);
		
//		Diagramm2D d2 = new Diagramm2D(m.getWidth() - 100, m.getHeight() / 2 - 150, -2f, -1f, 1f, 1f);
//		m.addDiagram2D(d2, 50, 100 + m.getHeight() / 2);
		
		m.build();
	}
	
	public static float f(float x){
		return -x * (x - 3) * (x - 6) * (x - 10);
	}
	
	public static float f2(float r){
		return (1f / r) * 4f * 10.22f * 1.38064852e-23f * (-pow(0.2556f / r, 12) + pow(0.2556f / r, 6));
	}
	
	//sin
	public static float f3(float x){
		return (float) Math.sin(x) + 1f;
	}
	
	public static float f4(float x){
		return (float) Math.cos(x) + 1f;
	}
	
	public static float pow(float f, int n){
		if(n == 0) return 1f;
		
		float ret = f;
		for(int i = 1; i < n; i++){
			ret *= f;
		}
		
		return ret;
	}
	
	public static Diagram loadFile(float[] x, float[] y, float[] y2, Animator m){
		try {
			Scanner sc = new Scanner(new File("table/v0_2m.dat"));
			String line;
			String[] comp;
			int i = 0;
			
			float xmax = 0f;
			float ymax = 0f;
			
			while(sc.hasNextLine()){
				line = sc.nextLine();
				comp = line.split("\t");
				
				x[i] = Float.parseFloat(comp[0]);
				y[i] = Float.parseFloat(comp[1]);
				y2[i] = Float.parseFloat(comp[2]);
				
				
				if(x[i]> xmax) xmax = x[i];
				if(y[i]> ymax) ymax = y[i];
				if(y2[i]> ymax) ymax = y2[i];
				i++;
			}
			
			sc.close();
			
			return new Diagram(m.getWidth() - 100, m.getHeight() / 2, xmax, ymax);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			return null;
		}
	}
}
