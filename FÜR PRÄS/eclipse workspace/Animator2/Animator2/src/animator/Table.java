package animator;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Scanner;

public class Table implements Serializable {

	private static final long serialVersionUID = 1L;
	
	public static Table loadByObjectFile(File file){
		Object obj = null;
		try {
			ObjectInputStream is = new ObjectInputStream(new FileInputStream(file));
			obj = is.readObject();
			is.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		
		return (Table) obj;
	}
	
	public static boolean writeTable(Table table, File out){
		try {
			ObjectOutputStream os = new ObjectOutputStream(new FileOutputStream(out));
			os.writeObject(table);
			os.close();
			return true;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return false;
	}
	
	public static Table loadByFile(File file, int... usingRows){
		Table t = null;
		float[][] table;
		float[] min;
		float[] max;
		
		try {
			Scanner sc = new Scanner(file);
			String line = sc.nextLine();
			String[] comps = line.split("\t");
			
			table = new float[usingRows.length][];
			min = new float[usingRows.length];
			max = new float[usingRows.length];
			HashMap<Integer, List<Float>> floats = new HashMap<Integer, List<Float>>();
			
			float f;
			
			for(int i = 0; i < usingRows.length; i++) {
				List<Float> l = floats.get(i);
				
				if(l == null){
					l = new ArrayList<Float>();
					floats.put(i, l);
				}
				
				l.add(f = Float.parseFloat(comps[usingRows[i]]));
				min[i] = f;
				max[i] = f;
			}
			
			while(sc.hasNextLine()){
				line = sc.nextLine();
				comps = line.split("\t");
				
				for(int i = 0; i < usingRows.length; i++) {
					List<Float> l = floats.get(i);
					
					l.add(f = Float.parseFloat(comps[usingRows[i]]));
					
					if(min[i] > f) min[i] = f;
					else if(max[i] < f) max[i] = f;
				}
			}
			
			sc.close();
			
			for(int i = 0; i < usingRows.length; i++){
				Object[] arr = floats.get(i).toArray();
				float[] a = new float[arr.length];
				
				for(int i2 = 0; i2 < a.length; i2++){
					a[i2] = (Float) arr[i2];
				}
				table[i] = a;
			}
			
			t = new Table(table, min, max);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
		return t;
	}
	
	public float[][] graphes;
	public float[] graphesMax;
	public float[] graphesMin;

	public Table(float[][] graphes, float[] min, float[] max) {
		this.graphes = graphes;
		this.graphesMax = max;
		this.graphesMin = min;
	}
	
	public Table(int numberOfGraphes) {
		graphes = new float[numberOfGraphes][];
	}
	
	public Table(int numberOfGraphes, int length) {
		graphes = new float[numberOfGraphes][length];
	}
	
	public void setGraph(int index, float[] content){
		graphes[index] = content;
	}
	
	public float getMax(int graph){
		return graphesMax[graph];
	}
	
	public float getMin(int graph){
		return graphesMin[graph];
	}
	
	public float getMax(int... graph){
		float max = 0f;
		
		for(int a : graph){
			if(max < graphesMax[a]) max = graphesMax[a];
		}
		
		return max;
	}
	
	public float getMin(int... graph){
		float min = 0f;
		
		for(int a : graph){
			if(min > graphesMin[a]) min = graphesMin[a];
		}
		
		return min;
	}
}
