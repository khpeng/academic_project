import java.awt.Color;
import java.awt.Graphics;

public class Ball{
  
  private int x, y;
  private int dX = 1, dY = 1;
  public static final int RADIUS = 15;
  public static final Color BALL_COLOR = Color.RED;
  
  public Ball(int x, int y, int dX, int dY) {
    this.x = x;
    this.y = y;
    this.dX = dX;
    this.dY = dY;
  }
  
  public int getX() {
    return x;
  }
  public void setX(int x){
    this.x = x;
  }
  public int getY() {
    return y;
  }
  public void setY(int y){
    this.y = y;
  }
    public int getdX() {
    return dX;
  }
  public void setdX(int dX){
    this.dX = dX;
  }
  public int getdY() {
    return dY;
  }
  public void setdY(int dY){
    this.dY = dY;
  }
  
  public void move() {
    this.x += dX;
    this.y += dY;
  }
  
  public void drawBall (Graphics g) {
     g.setColor(BALL_COLOR);
     g.fillOval(x, y, RADIUS * 2, RADIUS * 2);
     g.setColor(Color.BLACK);
     g.drawOval(x, y, RADIUS * 2, RADIUS * 2);
  }
  
  public static void main(String[] args) {
   
  }
}