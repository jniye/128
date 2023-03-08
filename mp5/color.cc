#include "color.hpp"


Color::Color(int r, int g, int b): red_(r), green_(g), blue_(b) {
  const int kMAX = 255;
  const int lMIN = 0;
  if(r < lMIN || r > kMAX || g < lMIN || g > kMAX || b < lMIN || b > kMAX){
    throw std::runtime_error("Invalid color");
  }
}

// do not modify
bool operator==(const Color& rhs, const Color& lhs) {
  return (rhs.Red() == lhs.Red() && rhs.Green() == lhs.Green() &&
          rhs.Blue() == lhs.Blue());
}