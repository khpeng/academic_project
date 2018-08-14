import java.awt.Color;
import java.awt.Graphics;

public class Brick {

  public static final int BRICK_WIDTH = 75;
  public static final int BRICK_HEIGHT = 30;
  private int x,y; 
  private Color c;
  
  public Brick(int x, int y, Color c) {
    this.x = x;
    this.y = y;
    this.c = c;
  }
  
  public int getX() { 
    return x;
  }
  
  public int getY(){
    return y;
  }
  
  public Color getColor() {
    return c;
  }
  
  public boolean hitted (Ball b) {
    if(b.getX() <= (x + BRICK_WIDTH) && b.getX() >= x){
      if(b.getY() <= (y + BRICK_HEIGHT) && b.getY() >= (y + (BRICK_HEIGHT / 2))){
        b.setdY (b.getdY() * -1);
        return true;
      }    
      else if(b.getY() >= (y - Ball.RADIUS *2) && b.getY() < (y + (Ball.RADIUS *2 / 3))){
        b.setdY ( b.getdY() * -1);
        return true;
      }
    } 
    else if(b.getY() <= (y + BRICK_HEIGHT) && b.getY() >= y){
      if(b.getX() <= (x + BRICK_WIDTH) && b.getX() > (x + (BRICK_WIDTH - Ball.RADIUS))){
        b.setdX( b.getdX() * -1);
        return true;
      }
      else if(b.getX() >= (x - Ball.RADIUS*2) && b.getX() < (x + Ball.RADIUS)){
        b.setdX( b.getdX() * -1);
        return true;
      }
    }
    return false;
  }
    public void drawBrick(Graphics g){
        g.setColor(c);
        g.fillRect(x, y, BRICK_WIDTH, BRICK_HEIGHT);
        g.setColor(Color.WHITE);
        g.drawRect(x, y, BRICK_WIDTH, BRICK_HEIGHT);
    }
}
