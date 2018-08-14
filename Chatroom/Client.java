import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.concurrent.TimeUnit;
import java.util.Scanner;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.*;

public class Client{
 // using write to send msg to server
 static class Send extends Thread{
   public SocketChannel c;
   public Send(SocketChannel c) {
     this.c = c;
   }
   public void run() {
     ByteBuffer buffer = ByteBuffer.allocate(4000);
     // scan input
     Scanner s = new Scanner(System.in); 
     while(true){
        System.out.println("Command: login, logout, register, sendp, list, sendap, sende, sendnp");
       while(s.hasNext()){
         //reading input
         String msg = s.nextLine();
         /*
         for (int i = 0; i < messages.length; i++) {
           byte [] message = new String(messages[i]).getBytes();
           ByteBuffer buffer =  ByteBuffer.wrap(message);
           client.write(buffer);
           System.out.println(messages [i] +  "\n");
           buffer.clear();
           TimeUnit.SECONDS.sleep(5);
          */
         buffer.put(msg.getBytes());
	 buffer.flip();
         try{
         c.write(buffer);
         } catch (IOException e) {}
         buffer.clear();
//         try{
//         TimeUnit.SECONDS.sleep(5);
//         } catch (InterruptedException e) {}
         }
       }
     }
  }
  
   static class Recv extends Thread{
    public SocketChannel c;
    public Recv(SocketChannel c){
     this.c = c; 
    }
    
    public void run() {
      ByteBuffer buffer = ByteBuffer.allocate(4000);
      while(true){
        if(c.isConnected()) {
          int length = 0;
          int offset = 0;
          byte[] b = new byte[4000];
          try{  
         	length = c.read(buffer);
	} catch (IOException e) {}
          if(length  > 0) {
 	    buffer.flip();
            buffer.get(b, offset, length +offset);
            offset += length;
          }
          String msg = new String(b, 0, offset);
          if(msg.equals("logout")) {
            try {
            c.close();
            } catch (IOException e) {}
            System.out.println("logout\n");
            break;
          }
          System.out.println(msg + "\n");
          buffer.clear();
//	  System.out.println("Command: login, logout, register, sendp, list, sendap, sende, sendnp");
        } else {
          System.out.println("Connection Error");
          break;
        }
      }
    }
  }
  public static void main(String[] args) throws IOException, InterruptedException {
    InetSocketAddress addr =  new InetSocketAddress("52.14.53.73", 6001);
    SocketChannel client =  SocketChannel.open(addr);
    Send send = new Send(client);
    Recv recv = new Recv(client);
    send.start();
    recv.start();
  }
  
  
  
  
  
  
  
  
}
