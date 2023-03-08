#include "grayscale_image.hpp"

GrayscaleImage::GrayscaleImage(const std::string& filename, size_t width, size_t height){
    ElevationDataset elevation(filename,width,height);
    height_ = elevation.Height();
    width_ = elevation.Width();
    for(size_t i = 0; i < height_; i++){
        std::vector<Color> cvector;
        for(size_t j = 0; j < width_; j++){
            double n = elevation.DatumAt(i,j) - elevation.MinEle();
            double d = elevation.MaxEle() - elevation.MinEle();
            int shade_of_gray = 0;
            if(n == 0 || d == 0){
                Color c(shade_of_gray,shade_of_gray,shade_of_gray);
                cvector.push_back(c);
            }else{
            double value = n / d;
            shade_of_gray = (int) std::round(value * kMaxColorValue);
            Color c(shade_of_gray,shade_of_gray,shade_of_gray);
            cvector.push_back(c);
            }
        }
        image_.push_back(cvector);
    }
}

GrayscaleImage::GrayscaleImage(const ElevationDataset& dataset){
    height_ = dataset.Height();
    width_ = dataset.Width();
    for(size_t i = 0; i < height_; i++){
        std::vector<Color> cvector;
        for(size_t j = 0; j < width_; j++){
            double n = dataset.DatumAt(i,j) - dataset.MinEle();
            double d = dataset.MaxEle() - dataset.MinEle();
            int shade_of_gray = 0;
            if(n == 0 || d == 0){
                Color c(shade_of_gray,shade_of_gray,shade_of_gray);
                cvector.push_back(c);
            }else{
            double value = n / d;
            int shade_of_gray = (int) std::round(value * kMaxColorValue);
            Color c(shade_of_gray,shade_of_gray,shade_of_gray);
            cvector.push_back(c);
            }
        }
        image_.push_back(cvector);
    }
}


void GrayscaleImage::ToPpm( const std::string& name ) const{
    std::ofstream out(name);
    out << "P3" << std::endl;
    out << width_ << " " << height_ << std::endl;
    out << MaxColorValue() << std::endl;
    for(int i = 0; i < (int)height_; i++){
        for (int j = 0; j < (int)width_; j++){
            out << ColorAt(i,j).Red() << " " << ColorAt(i,j).Green() << " " << ColorAt(i,j).Blue();
            if(j != (int)width_ - 1){
                out << " ";
            }
        }
        out << std::endl;
    }
    out.close();
}

size_t GrayscaleImage::Height() const{
    return height_;
}

size_t GrayscaleImage::Width() const{
    return width_;
}

unsigned int GrayscaleImage::MaxColorValue() const{
    return kMaxColorValue;
}

const std::vector<std::vector<Color> >& GrayscaleImage::GetImage() const{
    return image_;
}

const Color& GrayscaleImage::ColorAt(int row, int col) const{
    return image_.at(row).at(col);
}