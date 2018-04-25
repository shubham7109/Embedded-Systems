package sample;

import java.io.IOException;
import java.io.OutputStream;

public class SerialWriter {
	
	OutputStream out;

	public SerialWriter(OutputStream out) {
		this.out = out;
	}
	
	public void moveFront(){
		try {
			out.write('w');
		} catch (IOException e) {
			System.out.println(e.getMessage());
		}
	}
	
	public void moveBack(){
		try {
			out.write('s');
		} catch (IOException e) {
			System.out.println(e.getMessage());
		}
	}
	
	public void rotateCW(){
		try {
			out.write('d');
		} catch (IOException e) {
			System.out.println(e.getMessage());
		}
	}
	
	public void rotateCCW(){
		try {
			out.write('a');
		} catch (IOException e) {
			System.out.println(e.getMessage());
		}
	}
	
	public void sweep(){
		try {
			out.write('o');
		} catch (IOException e) {
			System.out.println(e.getMessage());
		}
	}

}
