class SignalIndicator {

  //Position variables
  int x, y, size, bars;
  
  //Data variables
  int signalStrength;
  float responseTime;
  float lastResponseTime;
  
  //Color variables
  color signalColor = color(160);
  color noSignalColor = color(80);
  color signalAlertColor = color(215, 60, 60);
  
  SignalIndicator(int tx, int ty, int tsize, int tbars) {
    x = tx;
    y = ty;
    size = tsize/tbars;
    bars = tbars;
  }
  
  void displaySignal() {
    
    //Background triangle
    noStroke();
    fill(noSignalColor);
    triangle(x, y + (size * bars), x + (size * bars), y, x + (size * bars), y + (size * bars));
    
    if(signalStrength > 0) {
      
      fill(signalColor);
      //textFont(SegoeUI);
      text("signal", x - textWidth("signal") - size, y + size * bars);
      
      if(signalStrength <= 1) {
        fill(signalAlertColor);
      } else {
        fill(signalColor);
      }
      
      //Draw indicator bars
      for(int i = 0; i < signalStrength; i++) {
        triangle(x + (i * size), y + (size * (bars - i)), x + ((i + 1) * size), y + (size * (bars - (i + 1))), x + ((i + 1) * size), y + (size * (bars - i)));
        rect(x + (i * size), y + (size * (bars - i)), size, size * i);
      }
    } else {
      //If no communications display no signal
      fill(signalAlertColor);
      //textFont(SegoeUI);
      text("no comm.", x - textWidth("no comm.") - size, y + size * bars);
    }
  }
  
  void update(float respTime) {
    final float maxRespTime = 200;
    
    for (int i = 0; i <= bars; i++) {
      responseTime = respTime * 0.1 + lastResponseTime * 0.9;
      
      if ( maxRespTime - responseTime < (maxRespTime/bars) * i) {
        signalStrength = i;
        break;
      }
    }
    lastResponseTime = responseTime;
    
    displaySignal();
  }
}
