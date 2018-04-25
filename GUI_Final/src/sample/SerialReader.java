package sample;

import java.io.IOException;
import java.io.InputStream;

public class SerialReader implements Runnable {

	InputStream in;
	int c;
	
	public SerialReader(InputStream in) {
		this.in = in;
		
	}

	@Override
	public void run() {
		try {
			while ((c = in.read()) != -1) {
			      System.out.print((char) c);
			    }
		} catch (IOException e) {
			System.out.println(e.getMessage());
		}
	}

}
