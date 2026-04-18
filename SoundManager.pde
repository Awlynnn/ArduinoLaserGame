import processing.sound.*;

public class SoundManager 
{

  private SoundFile introSound;
  private SoundFile mainSound;
  private SoundFile endingSound;
  private SoundFile shootingSound;

  public SoundManager(PApplet parent) 
  {
    // Loads
    introSound  = new SoundFile(parent, "intro.mp3");
    mainSound   = new SoundFile(parent, "main.mp3");
    endingSound = new SoundFile(parent, "ending.mp3");
    shootingSound = new SoundFile(parent, "shooting.mp3");

    // volumes
    introSound.amp(0.3);
    mainSound.amp(0.7);
    endingSound.amp(0.7);
    shootingSound.amp(1);
  }

  public void play(gameState state) 
  {

    stop(state); // Stop any currently playing sound

    switch (state) 
    {

      case intro:
        if (!introSound.isPlaying()) 
        {
          introSound.play();
        }
        break;

      case main:
        if (!mainSound.isPlaying()) 
        {
          mainSound.play();
        }
        break;

      case ending:
        if (!endingSound.isPlaying()) 
        {
          endingSound.play();
        }
        break;
        
    }
  }

  public void stop(gameState state) 
  {
    if (introSound.isPlaying() && state == gameState.main)  
    {
      introSound.stop();
    }
    
    if (mainSound.isPlaying() && state == gameState.ending) 
    {
      mainSound.stop();
    }
    if (endingSound.isPlaying() && state == gameState.intro)
    {
      endingSound.stop();
    }
  }
  public void shoot()
  {
    shootingSound.play();
  }
  
}
