import processing.net.*;
import processing.serial.*;
import processing.sound.*;


Serial myPort; 

Client c;


int clientRead;
String line;

int difficulty = 0;
int diff = 0;

int mainStates = 1;

//Image variables/objects
SoundManager sound;
ImageManager images;

//Game logic variables
boolean shooting = false;
int misses = 3; //change this according to difficulty
int stateKeyPress = 0;

//Game state
public enum gameState {
    intro  { 
      public gameState nextState() { return main;} 
    },
    main   {
      public gameState nextState() { return ending;} 
    },
    ending { 
      public gameState nextState() { return intro;} 
    };
    
    public abstract gameState nextState();
}
gameState state;

void setup()
{
  c = new Client(this, "130.229.159.250", 4444);
  myPort = new Serial(this, "COM5", 9600);
  
  //Screen logic
  size(1920,1080);
  
  //Class declarations
  images = new ImageManager(this);
  sound = new SoundManager(this);
  
  state = gameState.intro;
  
  println("initalised");
}


void draw() 
{  
  images.display(state);
  sound.play(state);
  
  
  if (state == gameState.intro)
  {
    //println(line); denna funkar rn
    //This sends the US values to the first Node remember to parse these values below, and only send US
    diff = difficulty;
    c.write(line);
    
    if (c.available() > 0)
    { 
      //We read values sent by the server
      clientRead = c.read();
      //println(clientRead);
      
      //If the server processes that they are ready, we procced to the next game state
    }
    
    if (clientRead == 1)
    {
      println(clientRead);
      state = state.nextState();
    }
   }


  else if (state == gameState.main)
  {
    
    if (mainStates == 1)
    {
      if (misses < 0)
      {
        state = gameState.ending;
        return;
      }
      else
      {
        int dir = direction();
        println("sent direction :" + dir);
  
        myPort.write(dir);
        
        delay(2500 * difficulty/3);
  
        c.write(dir);
        
        
        println("sent direction, changing state");
        sound.shoot();
        mainStates +=1;
      }
      
      
    }

    
    if (mainStates == 2)
    { 
      if (c.available() > 0)
      {
        if (c!= null)
        {
          clientRead = c.read();
          println("read server value");
          mainStates +=1;
          
        }
      }
    }
     
    if (mainStates == 3)
    {
      
      println("shooting, read clients value " + clientRead);
  
      if (clientRead == 1)
      {
        println("hit");
      }
      else if (clientRead == 0)
      {
        misses--;
        println("lives left " + misses);
      }
      
      mainStates = 1;
    }
    
    delay(1000 * difficulty);
  }
    
  
  else if (state == gameState.ending)
  {
    textSize(128);
    text("You lost", 40, 120); 
    
  }
}


public int direction()
{
  return int(random(1,6)); 
  
}

// Events from our serial port. Extend this to take values from both the accelerometer and the ultrasonic sensor.
void serialEvent(Serial p)
{
  String i = p.readStringUntil('\n');
  if (i != null)
  {
    i = i.trim();
    String[] values = i.split(",");
    if (values.length == 2)
    {
       line = values[0];
       if (int(values[1]) > 0)
       {
         difficulty = int(values[1]);
       }
       
    }
  }
}
