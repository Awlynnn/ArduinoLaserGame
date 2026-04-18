import processing.net.*;
import processing.serial.*;
import processing.sound.*;
Serial myPort;

//values of accelerometer
float x_value;
float y_value;
float z_value;

//for game
Server s;
Client c;
float ultrasonic;
String line;
int startGame = 0;
int photo = 0;
int acc; //accelerometer value

//variables for sound
SoundFile on_sound;
SoundFile block_sound;
SoundFile fail_sound;

int gameState = 1;
int direction; //angle of the projectile

//setup
void setup(){
  s = new Server(this, 4444); //creates server
  String portName = Serial.list()[0]; //finds the com-port
  myPort = new Serial(this, portName, 9600); //creates new serial object to write to port
  size(400,400);
  
  //import sound files
  on_sound = new SoundFile(this, "on.mp3");
  block_sound = new SoundFile(this, "block.mp3");
  fail_sound = new SoundFile(this, "fail.mp3");
}

//game loop
void draw(){
  if (gameState == 1) { //gamestate 1 = when we search for game start
    c = s.available();
    
    if (c != null){
      ultrasonic = float(c.readString());
      println("ultra sonic: " + ultrasonic);
      
      if (photo == 1 && ultrasonic > 20) { //if photointerrupter senses interruption and & ulta sonic sensor is in range
        startGame = 1;
        gameState = 2; //game start
        on_sound.play();
        myPort.write(1);
        delay(1000);
      }
      s.write(startGame);
   
    }
  }
  else if (gameState == 2)
  { //if game is already on
    c = s.available();
    if (c!=null) //when direction is recieved
    {
        direction = int(c.read()); // Direction state[1,2,3,4,5]
        //println(acc);
        println(direction);
       
        if (direction > 0 && direction < 6)
        {
          //Match the values of accelerometer and projectile
          if (acc == direction)
          { //successful block
            //green blinking
            block_sound.play();
            myPort.write(3);
            s.write(1);
            println("Sent blocked value");
          }
          else 
          { //blocking failed
            //red blinking
            fail_sound.play();
            myPort.write(2);
            s.write(0);
            println("Sent failed value");
          }
        }

    }    
  }
  
}


//event listener function
void serialEvent(Serial p){
  String i = p.readStringUntil('\n'); //trim line
  if (i != null)
  {
    i = i.trim();
    String[] values = i.split(",");
    if (values.length == 2)
    {
     photo = int(values[0]);
     acc = int(values[1]);
       
    }
  }
}
