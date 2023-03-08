#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

int SeamCarver::GetEnergy(int row, int col) const{
  int energy = 0;
  Pixel up (0, 0, 0);
  Pixel down (0, 0, 0);
  Pixel left (0, 0, 0);
  Pixel right (0, 0, 0);
  if(row == 0) {up = image_.GetPixel(height_ - 1 , col);}
  else {up = image_.GetPixel(row - 1 , col);}
  if(row == height_ - 1) {down = image_.GetPixel(0 , col);}
  else {down = image_.GetPixel(row + 1 , col);}
  if (col == 0) {left = image_.GetPixel(row, width_ - 1);}
  else {left = image_.GetPixel(row, col - 1);}
  if (col == width_ - 1) {right = image_.GetPixel(row, 0);}
  else {right = image_.GetPixel(row, col + 1);}
  int ud = (up.GetBlue() - down.GetBlue()) * (up.GetBlue() - down.GetBlue()) + (up.GetGreen() - down.GetGreen()) * (up.GetGreen() - down.GetGreen()) + (up.GetRed() - down.GetRed()) * (up.GetRed() - down.GetRed());
  int lr = (left.GetBlue() - right.GetBlue()) * (left.GetBlue() - right.GetBlue()) + (left.GetGreen() - right.GetGreen()) * (left.GetGreen() - right.GetGreen()) + (left.GetRed() - right.GetRed()) * (left.GetRed() - right.GetRed());
  energy = ud + lr;
  return energy;
}

#define MAX 2000000000

int SeamCarver::FindMin(int a, int b, int c) const{
  if(b <= a && b <= c) return b;
  else if(a < b && a <= c) return a;
  else return c;
}

int SeamCarver::FindMintwo(int a, int b) const{
  if(b <= a) return b;
  else return a;
}

int* SeamCarver::GetMinValueVer(int** val) const{
  int *path = new int [height_];
  int min = val[0][0];
  for(int i = width_ - 1; i >= 0; i--){
    if(val[0][i] <= min) {
      min = val[0][i]; 
      path[0] = i;
    }
  }
  for(int row = 1; row < height_; row++){
    int left = MAX;
    int middle = MAX;
    int right = MAX;
    if(path[row - 1] == 0){
      right = val[row][path[row - 1] + 1];
    }
    else if(path[row - 1] == width_ - 1){
      left = val[row][path[row - 1] - 1];
    }
    else{
      left = val[row][path[row - 1] - 1];
      right = val[row][path[row - 1] + 1];
    }

    middle = val[row][path[row - 1]];

    if(middle <= left && middle <= right){
      path[row] = path[row - 1];
    }
    else if(left < middle && left <= right){
      path[row] = path[row - 1] - 1;
    }
    else if(right < left && right < middle){
      path[row] = path[row - 1] + 1;
    }
  }
  for(int row = 0; row < height_; row++){
    delete[] val[row];
  }
  delete[] val;
  val = nullptr;

  return path;
}

int* SeamCarver::GetVerticalSeam() const{
  int **value = new int*[height_];
  for(int row = 0; row < height_; row++){
    value[row] = new int[width_];
  }
  int best = MAX;
  for(int col = 0; col < width_; col++){
    value[height_ - 1][col] = GetEnergy(height_ - 1, col);
  }
  for(int row = height_ - 2; row >= 0; row--){
    for(int col = 0; col < width_; col++){
      if(col == 0) {
        best = FindMintwo(value[row + 1][col], value[row + 1][col + 1]);
        value[row][col] = best + GetEnergy(row, col);
        continue;
      }
      if(col == width_ - 1){
        best = FindMintwo(value[row + 1][col - 1], value[row + 1][col]);
        value[row][col] = best + GetEnergy(row, col);
        continue;
      }
      best = FindMin(value[row + 1][col - 1], value[row + 1][col], value[row + 1][col + 1]);
      value[row][col] = best + GetEnergy(row, col);
    }
  }
  return GetMinValueVer(value);
}

int* SeamCarver::GetMinValueHor(int** val) const{
  int *path = new int [width_];
  int min = MAX;
  for(int i = height_ - 1; i >= 0; i--){
    if(val[i][0] <= min) min = val[i][0];
  }
  for(int i = 0; i <height_; i++){
    if(val[i][0] == min) {path[0] = i; break;}
  }
  for(int col = 1; col < width_; col++){
    int up = MAX;
    int middle = MAX;
    int down = MAX;
    if(path[col - 1] == 0){
      down = val[path[col - 1] + 1][col];
    }
    else if(path[col - 1] == height_ - 1){
      up = val[path[col - 1] - 1][col];
    }
    else{
      up = val[path[col - 1] - 1][col];
      down = val[path[col - 1] + 1][col];
    }
    middle = val[path[col - 1]][col];
    if(middle <= up && middle <= down){
      path[col] = path[col - 1];
    }
    else if(up < middle && up <= down){
      path[col] = path[col - 1] - 1;
    }
    else if(down < up && down < middle){
      path[col] = path[col - 1] + 1;
    }
  }
  for(int row = 0; row < height_; row++){
    delete[] val[row];
  }
  delete[] val;
  val = nullptr;
  return path;
}

int* SeamCarver::GetHorizontalSeam() const{
  int **value = new int*[height_];
  for(int row = 0; row < height_; row++){
    value[row] = new int[width_];
  }
  int best = MAX;
  for(int row = 0; row < height_; row++){
    value[row][width_ - 1] = GetEnergy(row, width_ - 1);
  }
  for(int col = width_ - 2; col >= 0; col--){
    for(int row = 0; row < height_; row++){
      if(row == 0) {
        best = FindMintwo(value[row][col + 1], value[row + 1][col + 1]);
        value[row][col] = best + GetEnergy(row, col);
        continue;
      }
      if(row == height_ - 1){
        best = FindMintwo(value[row - 1][col + 1], value[row][col + 1]);
        value[row][col] = best + GetEnergy(row, col);
        continue;
      }
      best = FindMin(value[row + 1][col + 1], value[row][col + 1], value[row - 1][col + 1]);
      value[row][col] = best + GetEnergy(row, col);
    }
  }
  return GetMinValueHor(value);
}

void SeamCarver::RemoveHorizontalSeam(){
  image_.RemoveHorizontalSeam(GetHorizontalSeam());
  height_--;
}

void SeamCarver::RemoveVerticalSeam(){
  int *seam = GetVerticalSeam();
  width_--;
  image_.setWidth(width_);
  Pixel** pix = new Pixel*[height_];
  for (int i = 0; i < height_; i++) {
    pix[i] = new Pixel[width_];
  }
  for(int row = 0; row < height_; row++){
    for(int col = 0; col < width_; col++){
      if(seam[row] <= col){
        pix[row][col] = image_.GetPixel(row, col + 1);
      }
      else {pix[row][col] = image_.GetPixel(row,col);}
    }
  }
  image_.SetPixel(pix);
  //image_.RemoveVerticalSeam(GetHorizontalSeam());
  //width_--;
}




