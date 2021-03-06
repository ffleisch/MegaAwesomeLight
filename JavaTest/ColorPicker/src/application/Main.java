package application;
	
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

import javafx.application.Application;
import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.control.*;
import javafx.event.*;

public class Main extends Application {
	
	public static void sendCol(Socket mySocket,int r,int g,int b) throws IOException {
		mySocket.sendUrgentData('C');
		mySocket.sendUrgentData(r);
		mySocket.sendUrgentData(g);
		mySocket.sendUrgentData(b);
	}
	
	
	@Override
	public void start(Stage primaryStage) {
		try {
			ColorPicker colpick=new ColorPicker();
			Socket mySocket=new Socket("192.168.1.117",80);
			OutputStream out=mySocket.getOutputStream();
			
			 colpick.setOnAction(new EventHandler<ActionEvent>() {
			     public void handle(ActionEvent t) {
			         Color c = colpick.getValue();
			         try {
						sendCol(mySocket,(int)(c.getRed()*255),(int)(c.getGreen()*255),(int)(c.getBlue()*255));
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
			     }
			 });

			 
			 
			 Button test=new Button();
			test.setOnAction(new EventHandler<ActionEvent>() {
				
				@Override
				public void handle(ActionEvent event) {
					try {
						mySocket.sendUrgentData(255);
						mySocket.sendUrgentData(000);
						mySocket.sendUrgentData(000);
						mySocket.sendUrgentData(255);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			});
			 test.layoutXProperty().set(100);
			GridPane root = new GridPane();
			root.getChildren().addAll(colpick,test);
			Scene scene = new Scene(root,400,400);
			scene.getStylesheets().add(getClass().getResource("application.css").toExternalForm());
			primaryStage.setScene(scene);
			primaryStage.show();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) {
		launch(args);
	}
}
