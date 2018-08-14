import java.io.IOException;
import java.net.InetSocketAddress;
//import java.net.Socket;
//import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.io.*;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.*;


public class  Server{
  
   static Map<SocketChannel, User> usermap = new HashMap<SocketChannel, User>(); // to store users who logined;
   
  // create object User to store info for password and username
  static class User{
     public String username, password;
     public User(String[] str){
       username = str[0];
       password = str[1];
       //tips = str[3];
     }
     public boolean checkPassword(String password) {
       return this.password.equals(password);
     }
     
     public String getUser(){
       return username;
     }
   }
 //creating hashmap and use it in login and register function
  static class Data{
    //import data from info.txr file
    public Map<String, User> load() {
      Map<String, User> map = new HashMap<String, User>();
      try {
        Scanner s = new Scanner(new File("info.txt"));           
        while (s.hasNextLine()) {
          String[] split_string = s.nextLine().split(",");
          User user = new User(split_string);
          map.put(user.username, user);
        }                   
      } catch (FileNotFoundException e) {
        System.out.println(e.getClass());
        
      }
      return map;
    }
  }
  
  // use write to send message
  public static void send_client(SocketChannel c, String msg) throws IOException{
    ByteBuffer buffer = ByteBuffer.allocate(3999);
    buffer.put(msg.getBytes());
    buffer.flip();
    c.write(buffer);
    System.out.println("send_client function send to client:" + msg);
    buffer.clear();
  }
  
  //login method. format: login [username] [password]
  public static void login(SocketChannel c, String[] msg) throws IOException {   
    //if argument format is wrong
    if(msg.length != 3) {
      send_client(c, "Login-argument format: login [usernmae] [password]\n");
      return;
    }
    //format right 
    //load data from info.txt
    Data data = new Data();
      Map<String, User> map = data.load();
      String username = msg[1];
      String password = msg[2];
      //check if data has username 
      if(map.containsKey(username)){ 
        //check if password match
        if (map.get(username).checkPassword(password)) {
          //success
          System.out.println(username + ": login success\n");
          send_client(c, "Login success. Start a conversation now.\n");
          String[] str1 = new String [] {username, password};
          
          User user = new User(str1);
          usermap.put(c, user);
        } else {//fail
          send_client(c, "Incorrect password\n");
        }
      } else {
        send_client(c, "That username does not exist.\n");
    }
  }
  //register for user. Format: register [usernmae] [password]
  public static void register(SocketChannel c, String[] msg) throws IOException {  
    //if argument format is wrong
    if(msg.length != 3) {
      send_client(c, "register-argument format: register [usernmae] [password]\n");
      return;
    } 
    Data data = new Server.Data();
    Map<String, User> map = data.load();
    String username = msg[1];
    String password = msg[2];
    if(map.containsKey(username)){ //user already registed
      send_client(c, "User already registed\n");
      return;
    } else { //everything correct
      String info = msg[1] + "," + msg[2];  
      try{
        File file = new File("info.txt");
        FileWriter fw = new FileWriter(file);
        fw.write(info);
        fw.close();
        
      } catch (IOException e) {
        e.printStackTrace();
      }
      send_client(c, "Regiter success. Please login.\n");
    }
  }
  //List all online user. format: listuser
  public static void listUser(SocketChannel c) {
    //check if user has permission
    if(usermap.containsKey(c)) {
      System.out.println("Print all user\n");
      try{
      send_client(c, "list all user\n");
      } catch (IOException e) {}
      for (Map.Entry<SocketChannel, User> entry : usermap.entrySet()) {
        User u = entry.getValue();
        System.out.println("User: " + u.username +"\n");
      }
    } else {
	try {
        send_client(c, "Please login"); 
        } catch (IOException e) {}
	}
  }
      
  
  
  // Logout.format logout
  public static void logout(SocketChannel c) throws IOException{
    //check if user has permission 
    if(usermap.containsKey(c)){
      User u = usermap.get(c);
      String user = u.username;
      usermap.remove(c);
      send_client(c, "User: " + user + " removed\n");
    } else {
      send_client(c,"Please Login. There is no such user to logout.\n");
      c.close();
    }
  }

  //send private msg. sendp [username] [msg]
  public static void sendPrivate(SocketChannel c, String[] msg) throws IOException{
    //check argument
    if(msg.length != 3 ){
      send_client(c, "sendPrivate-argument format: sendp [usernmae] [msg]\n");
      return;
    }
    //check permission
    if(usermap.containsKey(c)){
      User u1 = usermap.get(c);
      String user1 = u1.username;
      String user2 = msg[1];
      int found = 0;
      for (Map.Entry<SocketChannel, User> entry : usermap.entrySet()) {
        User u = entry.getValue();
        if(u.username.equals(user2)) {
          send_client(entry.getKey(), "User: "+ user1 + " sent you private message " + msg[2] + "\n");
          send_client(c, "Message sent\n");
          found = 1;
          return;
        }
      }
      if(found == 0) { 
        send_client(c, "User: " + msg[1] + " not found\n");
      }
    }else { 
        send_client(c, "Please login"); 
        }
  }
  
  
  //send private anonymous msg. sendap [username] [msg]
  public static void sendaPrivate(SocketChannel c, String[] msg) throws IOException{
    //check argument
    if(msg.length != 3 ){
      send_client(c, "sendPrivateAnonymous-argument format: senda [usernmae] [msg]\n");
      return;
    }
    //check permission
    if(usermap.containsKey(c)){
      String user2 = msg[1];
      int found = 0;
      for (Map.Entry<SocketChannel, User> entry : usermap.entrySet()) {
        User u = entry.getValue();
        if(u.username.equals(user2)) {
          send_client(entry.getKey(), "Someone sent private message:" + msg[2] + "\n");
          send_client(c, "Message sent\n");
          found = 1;
          return;
        }
      }    

      if(found == 0) { 
        send_client(c, "User: " + msg[1] + " not found\n");
      }
    } else {
	send_client(c, "Please login");
	}
  }
  
  //send public msg. sende [msg]
  public static void sendPublic(SocketChannel c, String[] msg) throws IOException{
    //check argument
    if(msg.length != 2 ){
      send_client(c, "sendPublic-argument format: send [msg]\n");
      return;
    }
    //check permission
    if(usermap.containsKey(c)){
      User u1 = usermap.get(c);
      String user1 = u1.username;
      for (Map.Entry<SocketChannel, User> entry : usermap.entrySet()) {
        //SocketChannel temp = entry.getKey();
        send_client(entry.getKey(), "User: "+ user1 + " sent public message: " + msg[1] + "\n");
      }
      send_client(c,"Public message sent\n");
    }
  }
  
    //send anonymous public msg. sendnp [msg]
  public static void sendaPublic(SocketChannel c, String[] msg) throws IOException{
    //check argument
    if(msg.length != 2 ){
      send_client(c, "sendPublic-argument format: send [msg]\n");
      return;
    }
    //check permission
    if(usermap.containsKey(c)){
      for (Map.Entry<SocketChannel, User> entry : usermap.entrySet()) {
        send_client(entry.getKey(), "Anonymous: sent public message: " + msg[1] + "\n");
      }
      send_client(c,"Anonymous public message sent\n");
    }
  }
  //message handle
  public static void msgHandle(SocketChannel c) throws IOException {
    ByteBuffer buffer = ByteBuffer.allocate(4000);
    int len = 0;
    String msg = "";
    if((len=c.read(buffer)) >0) {
	buffer.flip();
	
      byte[] b = new byte[6666];
      buffer.get(b, 0, len);
      System.out.println("Request good");
	System.out.println(new String(b, 0, len));
      msg = new String(b, 0, len);
	System.out.println(msg);
      System.out.println(msg);
    } else if( len < 0) {
      c.close();
      System.out.println("connection closed");
      return;
    }
    buffer.clear();
    String[] str = msg.split("\\s+");
    if(msg.startsWith("login")){
      System.out.println("login requested");
      login(c, str);
    } else if(msg.startsWith("register")){
      System.out.println("register requested");
      register(c, str);
    } else if(msg.startsWith("logout")){
      System.out.println("logout requested");
      logout(c);
    } else if(msg.startsWith("list")){
      System.out.println("list all user requested");
      listUser(c);
    } else if(msg.startsWith("sendp")){
      System.out.println("send private requested");
      sendPrivate(c, str);
    } else if(msg.startsWith("sendap")){
      System.out.println("send anonymous private requested");
      sendaPrivate(c, str);
    } else if(msg.startsWith("sende")){
      System.out.println("send public requested");
      sendPublic(c, str);
    } else if(msg.startsWith("sendnp")){
      System.out.println("send public requested");
      sendaPublic(c, str);
    } else {
      send_client(c, "Invaild command, Command: login, logout, register, sendp, list, sendap, sende, sendnp"); 
    }
  }
  
  public static void main(String[] args) throws IOException{
    //init    
    //create server socker channel
    ServerSocketChannel server = ServerSocketChannel.open();
    //set it to bind port 6001
    server.socket().bind(new InetSocketAddress(6001));
    //set nonblock
    server.configureBlocking(false);
    // init and set selector open
    Selector selector = Selector.open(); 
    server.register(selector, SelectionKey.OP_ACCEPT);
    while(true){
      selector.select();
      Iterator<SelectionKey> conns = selector.selectedKeys().iterator();
      //if there is new connection
      while (conns.hasNext()) {
        SelectionKey socket = conns.next();
        //socket.equals(1);
        // if new connection works
        if(socket.isAcceptable()){
          // get the new connection, nio, bind with read event
          System.out.println("New incoming connection");
          SocketChannel newSocket = server.accept();
          newSocket.configureBlocking(false);
          // add new client 
          newSocket.register(selector, SelectionKey.OP_READ);
        }
        
        // get the message from client
        try{
          if(socket.isReadable()){
            SocketChannel conn = (SocketChannel) socket.channel();
            msgHandle(conn);
          }
          conns.remove();
        } catch(IOException e){
          System.out.println (e.toString());
        }
      }
    }
  }
}


