#include "path_image.hpp"
#define MAX 100000

PathImage::PathImage(const GrayscaleImage &image, const ElevationDataset &dataset){
    width_ = image.Width();
    height_ = image.Height();
    path_image_ = image.GetImage();
    Path best_path(width_, 0);
    unsigned int min = MAX;
    for(size_t i = 0; i < height_; i++){
        Path p(width_,i);
        p.SetLoc(0,i);
        size_t r = i;
        for(size_t j = 1; j < width_; j++){
            int a = MAX, b = MAX, c = MAX;
            if(r > 0 && r < height_ - 1){
            a = abs(dataset.DatumAt(r,j) - dataset.DatumAt(r ,j - 1));
            b = abs(dataset.DatumAt(r - 1, j) - dataset.DatumAt(r ,j - 1));
            c = abs(dataset.DatumAt(r + 1, j) - dataset.DatumAt(r ,j - 1));
            }else if(r == 0){
                a = abs(dataset.DatumAt(r,j) - dataset.DatumAt(r ,j - 1));
                c = abs(dataset.DatumAt(r + 1, j) - dataset.DatumAt(r ,j - 1));
            }else if(r == height_ - 1){
                a = abs(dataset.DatumAt(r,j) - dataset.DatumAt(r ,j - 1));
                b = abs(dataset.DatumAt(r - 1, j) - dataset.DatumAt(r ,j - 1));
            }
            if(a <= b && a <= c){
                p.SetLoc(j , r);
                p.IncEleChange(a);
            }else if (c <= b && c <= a){
                p.SetLoc(j , r + 1);
                r = r + 1;
                p.IncEleChange(c);
            }else if (b < c && b < a){
                p.SetLoc(j , r - 1);
                r = r - 1;
                p.IncEleChange(b);}}
        paths_.push_back(p);
        if(min > p.EleChange()){
            min = p.EleChange();
            best_path = p;}}
    GetImage(best_path);
}

void PathImage::GetImage(Path &p1){
    const Color kRED(252,25,63);
    const Color kGREEN(31,253,13);
    for(int i = 0; i < (int)height_; i++){
        for(int j = 0; j < (int)width_; j++){
            int s = (int)paths_.at(i).GetPath().at(j);
            if(i == (int)p1.StartingRow()){
                continue;
                }else{
                    path_image_.at(s).at(j) = kRED;  
            }
        }
    }
    for(int j = 0; j < (int)width_; j++){
    int s = (int) paths_.at(p1.StartingRow()).GetPath().at(j);
    path_image_.at(s).at(j) = kGREEN;
    }
}

void PathImage::ToPpm(const std::string& name) const{
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

const Color& PathImage::ColorAt(int row, int col) const{
    return path_image_.at(row).at(col);
}

size_t PathImage::Width() const{
    return width_;
}

size_t PathImage::Height() const{
    return height_;
}

unsigned int PathImage::MaxColorValue() const{
    return kMaxColorValue;
}

const std::vector<Path>& PathImage::Paths() const{
    return paths_;
}

const std::vector<std::vector<Color> >& PathImage::GetPathImage() const{
    return path_image_;
}