import processing.sound.*;

public class ImageManager 
{


  PImage introImage;
  PImage mainImage;
  PImage endingImage;
  
  private PApplet parent;

  public ImageManager(PApplet parent)
  {
    this.parent = parent;
    
    introImage = parent.loadImage("intro.jpg");
    mainImage = parent.loadImage("main.jpg");
    endingImage = parent.loadImage("ending.jpg");
  }

  public void display(gameState state) 
  {

    switch (state) 
    {

      case intro:
        parent.image(introImage,0,0,1920,1080);
        break;

      case main:
        parent.image(mainImage,0,0,1920,1080);
        break;

      case ending:
        parent.image(endingImage,0,0,1920,1080);
        break;
    }
  }
}
