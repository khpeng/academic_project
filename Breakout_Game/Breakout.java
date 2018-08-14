 
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.Timer;
import java.util.ArrayList;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseMotionListener;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import javax.swing.JButton;
import javax.swing.JLabel;


public class Breakout extends JPanel implements KeyListener, ActionListener {
  
  public static final int HEIGHT = 600;
  public static final int WIDTH = 600;
  private int count;
  private Timer timer;
  private Paddle player;
  private Ball ball;
  private int i,j;
  private ArrayList<Brick> bricks;
  private JButton b;
  private JLabel l;

  public Breakout() {
   super();
   
   
   player = new Paddle((WIDTH/2) -
                       (Paddle.PADDLE_WIDTH/2));
   
   ball = new Ball(((player.getX() + (Paddle.PADDLE_WIDTH / 2)) - Ball.RADIUS), 
                   (Paddle.yPos - (Ball.RADIUS*2 + 10)),
                   -5,
                   -5);
   bricks = new ArrayList<Brick>();
   
   count = WIDTH / Brick.BRICK_WIDTH;
   
  for(int i = 0; i < 4 ; ++i)
   for(int j = 0; j < count; ++j){
     bricks.add(new Brick((j * Brick.BRICK_WIDTH),
                          ((i+2) * Brick.BRICK_HEIGHT), 
                          RandomColor.getColor()));
       
      }
      
      
      addKeyListener(this);
      this.setFocusable(true);
      requestFocus();
      timer = new Timer(40, this);
      timer.start();
   }
  
  
  public void actionPerformed(ActionEvent e) {
    checkCollisions();
    ball.move();
   for(int i = 0; i < bricks.size(); ++i) {
      Brick b = bricks.get(i);
      }
      
    repaint();
  }
  
  private void checkCollisions() {
     if(player.hitPaddle(ball))  {
            ball.setdY(ball.getdY() * -1);
            return;
        }
    
    
    /* if (player.hitPaddle(ball) && ball.getX() >= player.getX() && ball.getX() < (player.getX() + Paddle.PADDLE_WIDTH *1/3)){
      
       ball.setdY(8 * -1); } 
         else if (player.hitPaddle(ball) && ball.getX() >= player.getX() + Paddle.PADDLE_WIDTH *1/3 && ball.getX() < (player.getX() + Paddle.PADDLE_WIDTH *2/3)) {
      
           ball.setdY(5 * -1);}
         else if (player.hitPaddle(ball) && ball.getX() >= player.getX() + Paddle.PADDLE_WIDTH *2/3 && ball.getX() < (player.getX() + Paddle.PADDLE_WIDTH )) {

           ball.setdY(8 * -1);  }*/
      
      
      
    
      
        if(ball.getX() >= (WIDTH - Ball.RADIUS*2) || ball.getX() <= 0){
            ball.setdX(ball.getdX() * -1);
        }
       
        if(ball.getY() <= 0){
            ball.setdY(ball.getdY() * -1);
        }
        if(ball.getY() > (HEIGHT - Ball.RADIUS*2)) {
        timer.stop();
        }
         
        
        for(int i = 0; i < bricks.size(); ++i){
          Brick b = bricks.get(i);
          if(b.hitted(ball)){
            player.setScore(player.getScore() + 50);
            bricks.remove(i);
                
          }
        }
  }
  
  private boolean empty() {
    
    if(bricks.size() != 0){
      return false;
    } 
    return true;
  }
   

     
  private boolean gameOver() {
    if(player.getLives() <= 1 ) {
      return true;
    } 
    else {
        return false;}
  }
    
    public void keyPressed(KeyEvent e) {
     // if(timer.isRunning()){
       //     return;
     switch (e.getKeyCode()) {
       case KeyEvent.VK_LEFT:
         player.setX(player.getX() - 15); break;
       case KeyEvent.VK_RIGHT:
         player.setX(player.getX() + 15); break;
       default: break;
     } 
     
        }
        
    
    public void keyTyped(KeyEvent e) {}
    public void keyReleased(KeyEvent e) {}
    
     public void paintComponent(Graphics g){
        super.paintComponent(g);
        g.clearRect(0, 0, WIDTH, HEIGHT);
        g.setColor(Color.WHITE);
        g.fillRect(0, 0, WIDTH, HEIGHT);
        player.drawPaddle(g);
        ball.drawBall(g);
       for (Brick b : bricks){
         b.drawBrick(g);}
       
        g.setColor(Color.BLACK);
        g.drawString("Score: " + player.getScore(), 10, 25);
        
        if(gameOver() &&
                ball.getY() > (HEIGHT - Ball.RADIUS*2)){
            JFrame f = new JFrame("Game Over!");
            f.setResizable(false);
            f.setVisible(true);
            f.setSize(400, 400);
            f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            JLabel l = new JLabel("You lost.");
            JButton b = new JButton ("You lost!         Score:" + player.getScore() + "    Click to exit.");
            b.addActionListener(new ButtonListener());
                                  
            
            l.setPreferredSize(new Dimension(50, 40));
            b.setPreferredSize(new Dimension(50, 40));
            f.add(l);
            f.add(b);
            // 
        }
          
            
        if(empty()){
            JFrame f = new JFrame("You won!");
            f.setResizable(false);
            f.setVisible(true);
            f.setSize(400, 400);
            f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            JLabel l = new JLabel("You Won.");
            JButton b = new JButton ("You won!      Score:"+ player.getScore() + "       Click to exit.");
            b.addActionListener(new ButtonListener());
                                  
            
            l.setPreferredSize(new Dimension(50, 40));
            b.setPreferredSize(new Dimension(50, 40));
            f.add(l);
            f.add(b);
            timer.stop();
        }
        
       
    }
     private class ButtonListener implements ActionListener  {
              public void actionPerformed(ActionEvent event) {
                Runtime.getRuntime().exit(1);
              }
            }
    
    
      public static void main(String[] args){
        JFrame frame = new JFrame();
        Breakout b = new Breakout();
        frame.add(b);
        frame.pack();
        frame.setResizable(false);
        frame.setVisible(true);
        frame.setSize(600, 700);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        
    }
}