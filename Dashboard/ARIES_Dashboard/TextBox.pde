class TextBox {

  int x, y, w, h;
  String label;
  String inputText;
  
  color cursorColorDark = color(100);
  color cursorColorLight = color(170);
  color selectedColor = color(60);
  color releasedColor = color(100);
  color hoverColor = color(80);
  boolean darkCursor = false;
  
  boolean selectable;
  boolean fitText;
  
  boolean locked = false;
  boolean pressed = false;
  boolean selected = false;
  boolean hover = false;
  boolean hasBeenPressed = false;
  boolean pressOriginInside = false;
  boolean lastPressed = false;
  boolean lastMouse = false;
  
  boolean charAdded = false;

  TextBox(int tx, int ty, int tw, int th, boolean tselectable, String tlabel) {
    x = tx;
    y = ty;
    w = tw;
    h = th;
    selectable = tselectable;
    label = tlabel;
  }
  
  TextBox(int tx, int ty, boolean tselectable, String tlabel) {
    x = tx;
    y = ty;
    selectable = tselectable;
    fitText = true;
    label = tlabel;
  }
  
  float update(String input) {
    if (selectable) {
      //mousePressed in text box
      if (mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h) && !locked) {
        if (mousePressed) {
          pressed = true;
          hover = false;
        } 
        else {
          hover = true;
          pressed = false;
        }
      } 
      else {
        hover = false;
        pressed = false;
      }
      
      //mouseDown in button
      if (mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h) && !locked && lastPressed == false && pressed == true && lastMouse == false && mousePressed) {
        pressOriginInside = true;
      } else if (!(mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h)) && !locked && lastPressed == false && pressed == true && lastMouse == false && mousePressed) {
        pressOriginInside = false;
      }
      
      //mouseReleased in button
      if (mouseX > x && mouseX < (x + w) && mouseY > y && mouseY < (y + h) && !locked && lastPressed == true && pressed == false && pressOriginInside) {
        hasBeenPressed = true;
      } else {
        hasBeenPressed = false;
      }
      
      lastPressed = pressed;
      lastMouse = mousePressed;
      
      //Handle selection
      if (pressed) {
        selected = true;
      } else if (mousePressed) {
        selected = false;
      }
      
      if (!keyDown) {
        charAdded = false;
      }
    }
    
    //Handle typing
    if (selected && !charAdded && keyDown) {
      println("KEYPRESSED");
      if (key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0') {
        inputText += key;
      }
      //Handle decimal
      if (key == '.' && !inputText.contains(".")) { 
        inputText += key;
      }
      //Handle backspace
      if (key == 8 && inputText.length() > 0) {
        inputText = inputText.substring(0,inputText.length() - 1);
      }
      //Handle newline
      if (key == '\n') {
        selected = false;
      }
      
      charAdded = true;
    } else if (!selected) {
      inputText = input;
    }
    
    drawTextBox();
    
    if (!(inputText.length() > 0)) {
      return 0.0;
    } else {
      return float(inputText);
    }
  }
  
  private void drawTextBox() {
    
      if(fitText) {
        //Calculate width and height
        w = int(textWidth(inputText));
        h = 20;
      }
      //Draw box
      strokeWeight(1);
      stroke(200);
      fill(50);
      if (w == 0) {
        w = int(textWidth(inputText) + 10);
      }
      
      if (selected) {
          fill(selectedColor);
        } else if (hover) {
          fill(hoverColor);
        } else {
          fill(releasedColor);
      }
        
      if (selected) {
        if (frameCount % 20 == 0) {
          if (darkCursor) {
            darkCursor = false;
          } else { 
            darkCursor = true;
          }
        }
        
        if (darkCursor) {
          stroke(cursorColorDark);
        } else {
          stroke(cursorColorLight);
        }
      }
      
      rect(x - 4, y , w + 6, h);
      
      fill(50);
      text(label, x - textWidth(label) - 5, y + h - 4);
      fill(#eeeeee);
      text(inputText, x + w - int(textWidth(inputText)), y + h-4);
  }
}

