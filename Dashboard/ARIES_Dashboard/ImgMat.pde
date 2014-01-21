// Provides nice mat behind images
// ImgMat Variables

class ImgMat {
  
  int x, y, border, tImgWidth, tImgHeight;
  PImage images[];
  
  ImgMat ( int x, int y, PImage images[]) {
    x = this.x;
    y = this.y;
    images = this.images;
    
    tImgHeight = images[0].height;
    for (int i = 0; i < images.length; i++) {
      tImgWidth += images[i].width;
      if (images[i].height > tImgHeight)
        tImgHeight = images[i].height;
    }
  }
  
  ImgMat (int x, int y, PImage[] images, int border) {
    x = this.x;
    y = this.y;
    images = this.images;
    border = this.border;
    
    tImgHeight = images[0].height;
    for (int i = 0; i < images.length; i++) {
      tImgWidth += images[i].width;
      if (images[i].height > tImgHeight)
        tImgHeight = images[i].height;
    }
  }
  
  void update() {
    noStroke();
    color(0);
    rect(x, y, tImgWidth + border * (images.length + 1), tImgHeight + border * 2);
    for (int i = 0; i < images.length; i++) {
      image(images[i], (images[i].width * i) + border, images[i].height + border);
      println(border);
    }
  }
  
}
