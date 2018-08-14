import java.awt.Graphics;
import java.awt.Color;
import java.awt.Rectangle;


public class Paddle{
  
  
  public static final int PADDLE_WIDTH = 90;
  public static final int PADDLE_HEIGHT = 20;
  public static final Color PADDLE_COLOR = Color.BLACK;
  private int x;
  public static final int DELTA_X = 5;
  private int score;
  public static final int yPos = Breakout.HEIGHT - 20;
  private int lives;
  
  public Paddle(int x) {
   this.x = x;
   score = 0;
   lives = 1;
   
  }
  public void setX(int x) {
   
   if (x > Breakout.WIDTH - PADDLE_WIDTH) {
      this.x = Breakout.WIDTH - PADDLE_WIDTH;
   }
   else if (x < 0 ) {
     this.x = 0;
   } else this.x = x; 
  }
  public int getX() {
    return x;
  }
  
  public int getScore() {
   return score;
  }
  public void setScore(int score) {
    this.score = score;
  }
  public int getLives(){ 
    return lives; 
  }
  public void setLives(int lives){ 
    this.lives = lives;
  }
  
  public boolean hitPaddle(Ball b) {
    if(b.getX() <= x + PADDLE_WIDTH + 20 ) {
      if(b.getX() >= x - 20){
        if((b.getY() + (Ball.RADIUS * 2 - 1) >= yPos)) {
          if((b.getY() + (Ball.RADIUS * 2 - 1)) <= (yPos + (PADDLE_HEIGHT - 15))) {
            return true;}}}}
    return false;}
  
  public void drawPaddle(Graphics g) {
   g.setColor(PADDLE_COLOR);
   g.fillRect(x, yPos, PADDLE_WIDTH, PADDLE_HEIGHT);
   g.setColor(Color.WHITE);
   g.drawRect(x, yPos, PADDLE_WIDTH, PADDLE_HEIGHT);
  }
  public static void main(String[] args) {

  
      
    
  }
}