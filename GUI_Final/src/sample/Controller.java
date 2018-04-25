package sample;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;


import java.io.BufferedReader;
import java.io.InputStreamReader;

    public class Controller {

        @FXML private TextArea output;
        @FXML private TextField input;
        private static Socket s;
        private SerialWriter writer;
        private SerialReader reader;
        private InputStream in;
        private int c=0;
        private Plotting plot;

        @FXML
        private void onEnter(ActionEvent ae){
            if(input.getText().length() == 1){
                if(input.getText().equals("w")){
                    output.appendText("Sending 'w' ...\n");
                    writer.moveFront();
                }
                else if(input.getText().equals("d")){
                    output.appendText("Sending 'd' ...\n");
                    writer.rotateCW();
                }
                else if(input.getText().equals("s")){
                    output.appendText("Sending 's' ...\n");
                    writer.moveBack();
                }
                else if(input.getText().equals("a")){
                    output.appendText("Sending 'a' ...\n");
                    writer.rotateCCW();
                }
                else if(input.getText().equals("o")){
                    output.appendText("Sending 'o' ...\n");
                    writer.sweep();
                }
                else
                {
                    output.appendText("Invalid char !\n");
                }

            }
            else
            {
                output.appendText("Length is not 1 character\n");
            }

            input.clear();
        }



        public void setLogs(char c2){
            output.appendText(c2 + "");
        }

        @FXML // This method is called by the FXMLLoader when initialization is complete
        void initialize() {
            output.setWrapText(true);
            output.setEditable(false);
            InetAddress addr;
            boolean loop = true;

            try {
                //addr= InetAddress.getByName("localhost");
                addr= InetAddress.getByName("192.168.1.1");
                SocketAddress sockaddr = new InetSocketAddress(addr,288);
                Socket sock = new Socket();
                sock.connect(sockaddr);
                output.setText("Connected!\n");
                in = sock.getInputStream();
                OutputStream out = sock.getOutputStream();
                writer = new SerialWriter(out);

            } catch (IOException e) {
                System.out.println(e.getMessage());
            }

            new Thread(new Runnable() {

                @Override
                public void run() {
                    try {
                        output.appendText("Reading...\n");
                        while (c != -1) {
                            setLogs((char) c);
                            c = in.read();
                        }
                    } catch (IOException e) {
                        System.out.println(e.getMessage());
                    }
                    output.appendText("Done reading \n");
                }

            }).start();

        }
    }
