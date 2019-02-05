package lightTest;
import java.io.IOException;
import java.io.OutputStream;
import java.net.*;
import java.util.Random;

public class program {
	public static void sendCol(Socket mySocket,int r,int g,int b) throws IOException {
		mySocket.sendUrgentData('C');
		mySocket.sendUrgentData(r);
		mySocket.sendUrgentData(g);
		mySocket.sendUrgentData(b);
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("Hello World");
		
		try {
			Random rand=new Random();
			Socket mySocket=new Socket("192.168.0.99",80);
			OutputStream out=mySocket.getOutputStream();
			mySocket.getInputStream();
			int r=0;
			int g=0;
			int b=0;
			for(int i=0;i<2255555;i++) {
				sendCol(mySocket,r,g,b);				
				r+=1;
				g+=1;
				b+=1;
			}
			Thread.sleep(10);

		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}catch (InterruptedException e) {
			 //TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
