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
  return max_color_value_;
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
  auto ** pix = new Pixel*[height_];
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
  for (int i = 0; i < height_ + 1; i++) {
    delete[] pixels_[i];
  }
  delete[] pixels_;
  delete[] seam;
  pixels_ = pix;
}

void ImagePPM::RemoveVerticalSeam(const int* seam){
  width_--;
  auto ** pix = new Pixel*[height_];
  for (int i = 0; i < height_; i++) {
    pix[i] = new Pixel[width_];
  }
  for(int row = 0; row < height_; row++){
    for(int col = 0; col < width_; col++){
      if(seam[row] <= col){
        pix[row][col] = pixels_[row][col + 1];
      }
      else {pix[row][col] = pixels_[row][col];}
    }
  }
  for (int i = 0; i < height_; i++) {
    delete[] pixels_[i];
  }
  delete[] pixels_;
  delete[] seam;
  pixels_ = pix;
}