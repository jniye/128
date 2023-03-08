#include "elevation_dataset.hpp"
#define MIN (-200000)
#define MAX 200000

ElevationDataset::ElevationDataset(const std::string& filename, size_t width, size_t height){
    std::ifstream ifs(filename);
    if(!ifs.is_open()){
        throw std::runtime_error("Invalid filename");
    }

    width_ = width;
    height_ = height;

    int cur = 0;
    max_ele_ = 0;
    min_ele_ = 0;
    size_t i = 0, j = 0;

    for(i = 0; i < height; i++){
        std::vector<int> row;
        for(j = 0; j < width; j++){
            ifs >> cur;
            if(ifs.fail()){
                    throw std::runtime_error("Shortage");
                    }else{
                        if(i==0 && j==0) {
                            max_ele_ = cur;
                            min_ele_ = cur;
                        }
                        if(cur > max_ele_) max_ele_ = cur;
                        if(cur < min_ele_) min_ele_ = cur;
                        row.push_back(cur);
                    }
        }
        data_.push_back(row);
    }
    ifs >> cur;
    if(ifs.good()){
        throw std::runtime_error("Overwhelmed");
    }
    
}

size_t ElevationDataset::Width() const{
    return width_;
}

size_t ElevationDataset::Height() const{
    return height_;
}

int ElevationDataset::MaxEle() const{
    return max_ele_;
}

int ElevationDataset::MinEle() const{
    return min_ele_;
}

int ElevationDataset::DatumAt(size_t row, size_t col) const{
    return data_.at(row).at(col);
}

const std::vector<std::vector<int> >& ElevationDataset::GetData() const{
    return data_;
}


