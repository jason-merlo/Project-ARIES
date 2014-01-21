float truncate(float x, int digits) {
  int d = (int)pow(10, digits);
  x *= d;
  x = int(x);
  x /= d;
  return x;
}

