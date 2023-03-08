#include "image_ppm.hpp"

// implement the rest of ImagePPM's functions here

// given functions below, DO NOT MODIFY

ImagePPM::ImagePPM(const std::string& path) {
  std::ifstream input_file(path);
  input_file >> *this;
  input_file.close();
}

ImagePPM::ImagePPM(const ImagePPM& source) { *this = source; }

ImagePPM& ImagePPM::operator=(const ImagePPM& source) {
  if (this == &source) {
    return *this;
  }

  Clear();

  height_ = source.height_;
  width_ = source.width_;
  max_color_value_ = source.max_color_value_;

  pixels_ = new Pixel*[height_];
  for (int row = 0; row < height_; row++) {
    pixels_[row] = new Pixel[width_];

    for (int col = 0; col < width_; col++) {
      pixels_[row][col] = source.pixels_[row][col];
    }
  }

  return *this;
}

ImagePPM::~ImagePPM() { Clear(); }

void ImagePPM::Clear() {
  for (int i = 0; i < height_; i++) {
    delete[] pixels_[i];
  }

  delete[] pixels_;

  height_ = 0;
  width_ = 0;
  pixels_ = nullptr;
}

std::istream& operator>>(std::istream& is, ImagePPM& image) {
  image.Clear();
  std::string line;
  // ignore magic number line
  getline(is, line);
  // check to see if there's a comment line
  getline(is, line);
  if (line[0] == '#') {
    getline(is, line);
  }
  // parse width and height
  int space = line.find_first_of(' ');
  image.width_ = std::stoi(line.substr(0, space));
  image.height_ = std::stoi(line.substr(space + 1));
  // get max color value
  getline(is, line);
  image.max_color_value_ = std::stoi(line);
  // init and fill in Pixels array
  image.pixels_ = new Pixel*[image.height_];
  for (int i = 0; i < image.height_; i++) {
    image.pixels_[i] = new Pixel[image.width_];
  }
  for (int row = 0; row < image.height_; row++) {
    for (int col = 0; col < image.width_; col++) {
      getline(is, line);
      int red = std::stoi(line);
      getline(is, line);
      int green = std::stoi(line);
      getline(is, line);
      int blue = std::stoi(line);

      Pixel p(red, green, blue);
      image.pixels_[row][col] = p;
    }
  }
  return is;
}

Pixel ImagePPM::GetPixel(int row, int col) const{
  return pixels_[row][col];
}

int ImagePPM::GetMaxColorValue() const{
  int max = 0;
  for(int i = 0; i < height_; i++){
    for (int j = 0; j < width_; j++){
      if(pixels_[i][j].GetBlue() > max) max = pixels_[i][j].GetBlue();
      if(pixels_[i][j].GetGreen() > max) max = pixels_[i][j].GetGreen();
      if(pixels_[i][j].GetRed() > max) max = pixels_[i][j].GetRed();
    }
  }
  return max;
}

std::ostream& operator<<(std::ostream& os, const ImagePPM& image){
    os << "P3" << std::endl;
    os << image.GetWidth() << " " << image.GetHeight() << std::endl;
    os << image.GetMaxColorValue() << std::endl;
    for(int i = 0; i < (int)image.GetHeight(); i++){
        for (int j = 0; j < (int)image.GetWidth(); j++){
            os << image.GetPixel(i,j).GetRed() << std::endl;
            os << image.GetPixel(i,j).GetGreen() << std::endl;
            os << image.GetPixel(i,j).GetBlue() << std::endl;
        }
    }
    return os;
}

void ImagePPM::RemoveHorizontalSeam(const int* seam){
  height_--;
  Pixel** pix = new Pixel*[height_];
  for (int i = 0; i < height_; i++) {
    pix[i] = new Pixel[width_];
  }
  for(int col = 0; col < width_; col++){
    for(int row = 0; row < height_; row++){
      if(seam[col] <= row){
        pix[row][col] = pixels_[row + 1][col];
      }
      else {pix[row][col] = pixels_[row][col];}
    }
  }
  for (int i = 0; i < height_+1; i++) {
    delete[] pixels_[i];
  }
  delete[] pixels_;
  pixels_ = pix;
  return;
}

0.5 2 8 81.2448 
1 0.5 8 73.5072 
0.5 4 8 96.72 
0.5 1 8 71.136 
4 2 16 164.736 
2 2 8 92.8512 
2 0.5 8 73.3824 
0.5 4 8 96.72 
4 0.5 8 93.6
4 1 16 162.4896
0.5 4 4 65.7696
4 1 8 100.5888
