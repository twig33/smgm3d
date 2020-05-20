#ifndef INPUT_HPP_
#define INPUT_HPP_

namespace Input {
  // Returns 1 if keyPositive is pressed, -1 if keyNegative is pressed, 0 if both are pressed or released
  int GetAxis(int keyPositive, int keyNegative);
  void GetMouseDelta(double* dx, double* dy);
  //returns true if key is pressed
  bool GetKey(int key);
  int Init();
  int Quit();
}

#endif
