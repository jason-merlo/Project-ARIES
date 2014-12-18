OpenCV ocvL, ocvR;
PImage  imgL, imgR, depth1, depth2;

void initStereo() {
  ocvL = new OpenCV(this, 320, 240);
  ocvR = new OpenCV(this, 320, 240);
}

void updateStereo() {
  ocvL.loadImage(camA);
  ocvR.loadImage(camB);
  
  ocvL.gray();
  ocvR.gray();
  Mat left = ocvL.getGray();
  Mat right = ocvR.getGray();

  Mat disparity = OpenCV.imitate(left);

  /* StereoSGBM(int minDisparity,
          int numDisparities,
          int SADWindowSize,
          int P1,
          int P2,
          int disp12MaxDiff,
          int preFilterCap,
          int uniquenessRatio,
          int speckleWindowSize,
          int speckleRange,
          boolean fullDP)
   */
  //StereoSGBM stereo =  new StereoSGBM(0, 16, 3, 32, 512, 128, 64, 24, 0, 0, true);
  
  StereoSGBM stereo =  new StereoSGBM(-32, 64, 3, 216, 864, -1, 16, 10, 50, 1, true);
  stereo.compute(left, right, disparity );

  Mat depthMat = OpenCV.imitate(left);
  disparity.convertTo(depthMat, depthMat.type());

  depth1 = createImage(depthMat.width(), depthMat.height(), RGB);
  ocvL.toPImage(depthMat, depth1);

  StereoBM stereo2 = new StereoBM();
  stereo2.compute(left, right, disparity );
  disparity.convertTo(depthMat, depthMat.type());


  depth2 = createImage(depthMat.width(), depthMat.height(), RGB);
  ocvL.toPImage(depthMat, depth2);
}
