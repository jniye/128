#include "db_table.hpp"

const unsigned int kRowGrowthRate = 2;

DbTable::DbTable(const DbTable& rhs){
    row_col_capacity_ = rhs.row_col_capacity_;
    next_unique_id_ = rhs.next_unique_id_;
    col_descs_ = rhs.col_descs_;
    for(unsigned int i = 0; i < next_unique_id_; i++){
        if(rhs.rows_.contains(i)){
            void ** new_data = new void*[rhs.row_col_capacity_];
            for(int j = 0; j < (int)col_descs_.size(); j++){
                if(rhs.col_descs_.at(j).second == DataType::kString)    new_data[j] = new std::string((*static_cast<std::string *>(rhs.rows_.at(i)[j])));
                if(rhs.col_descs_.at(j).second == DataType::kDouble)    new_data[j] = new double(*(static_cast<double *>(rhs.rows_.at(i)[j])));
                if(rhs.col_descs_.at(j).second == DataType::kInt)       new_data[j] = new int(*(static_cast<int*>(rhs.rows_.at(i)[j])));
            }rows_[i] = new_data;
        }
    }
}

DbTable& DbTable::operator=(const DbTable& rhs){
    if(this == &rhs) {return *this;}
    for(unsigned int i = 0; i < next_unique_id_; i++){
        if(rows_.contains(i)) {
            for(unsigned int j = 0; j < col_descs_.size(); j++){
                if(col_descs_.at(j).second==DataType::kString) delete static_cast<std::string*>(rows_.at(i)[j]);
                if(col_descs_.at(j).second==DataType::kInt) delete static_cast<int*>(rows_.at(i)[j]);
                if(col_descs_.at(j).second==DataType::kDouble) delete static_cast<double*>(rows_.at(i)[j]);
            } delete[] rows_.at(i);
            rows_.erase(i);
            }
    } rows_.clear();

    row_col_capacity_ = rhs.row_col_capacity_;
    next_unique_id_ = rhs.next_unique_id_;
    col_descs_ = rhs.col_descs_;
    for(unsigned int i = 0; i < next_unique_id_; i++){
        if(rhs.rows_.contains(i)){
            void ** new_data = new void*[rhs.row_col_capacity_];
            for(int j = 0; j < (int)col_descs_.size(); j++){
                if(rhs.col_descs_.at(j).second == DataType::kString)    new_data[j] = new std::string((*static_cast<std::string *>(rhs.rows_.at(i)[j])));
                if(rhs.col_descs_.at(j).second == DataType::kDouble)    new_data[j] = new double(*(static_cast<double *>(rhs.rows_.at(i)[j])));
                if(rhs.col_descs_.at(j).second == DataType::kInt)       new_data[j] = new int(*(static_cast<int*>(rhs.rows_.at(i)[j])));
            }rows_[i] = new_data;
        }
    }
    return *this;
}

void DbTable::AddColumn(const std::pair<std::string, DataType>& col_desc){
    //change col_descs_
    col_descs_.push_back(col_desc);
    //resize rows_
    if(col_descs_.size() > row_col_capacity_){
        row_col_capacity_  = row_col_capacity_ * kRowGrowthRate;
        if(rows_.empty()) return;
        //copy to new row
        for(unsigned int i = 0; i < next_unique_id_; i++){
            if(rows_.contains(i)){
            void ** new_row = new void* [row_col_capacity_];
            for(size_t j = 0; j < col_descs_.size() - 1; j++){
                if(col_descs_.at(j).second==DataType::kString){
                    new_row[j] = new std::string((*static_cast<std::string *>(rows_.at(i)[j])));
                    delete static_cast<std::string*>(rows_.at(i)[j]);
                }
                if(col_descs_.at(j).second==DataType::kInt){
                    new_row[j] = new int(*(static_cast<int*>(rows_.at(i)[j])));
                    delete static_cast<int*>(rows_.at(i)[j]);
                }
                if(col_descs_.at(j).second==DataType::kDouble){
                    new_row[j] = new double(*(static_cast<double *>(rows_.at(i)[j])));
                    delete static_cast<double*>(rows_.at(i)[j]);
                }
            }
            delete[] rows_.at(i);
            rows_[i] = new_row;
            }
        }
    }
    for(unsigned long i = 0; i < next_unique_id_; i++){
        if(rows_.contains(i)){
            if(col_desc.second==DataType::kString)  rows_.at(i)[col_descs_.size() - 1] = static_cast<void*>(new std::string(""));
            if(col_desc.second==DataType::kInt)     rows_.at(i)[col_descs_.size() - 1] = static_cast<void*>(new int(0));
            if(col_desc.second==DataType::kDouble)  rows_.at(i)[col_descs_.size() - 1] = static_cast<void*>(new double(0.0));
        }
    }
}

void DbTable::DeleteColumnByIdx(unsigned int col_idx){
    if(col_idx < 0 || col_idx >= col_descs_.size()){
        throw std::out_of_range("Invalid index");
    }
    if(col_descs_.size() == 1 && !rows_.empty()){
        throw std::runtime_error("Last column");
    }
    //delete from rows_
    for(unsigned long i = 0; i < next_unique_id_; i++){
        if(rows_.contains(i)){
        if(col_descs_.at(col_idx).second==DataType::kString)    delete static_cast<std::string*>(rows_.at(i)[col_idx]);
        if(col_descs_.at(col_idx).second==DataType::kInt)       delete static_cast<int*>(rows_.at(i)[col_idx]);
        if(col_descs_.at(col_idx).second==DataType::kDouble)    delete static_cast<double*>(rows_.at(i)[col_idx]);

        for(unsigned long j = col_idx; j < col_descs_.size() - 1; j++)  rows_.at(i)[j] =  rows_.at(i)[j + 1];
        rows_.at(i)[col_descs_.size() - 1] = nullptr;
        }
    }
    //delete from col_descs_
    col_descs_.erase(col_descs_.begin() + col_idx);
}


void DbTable::AddRow(const std::initializer_list<std::string>& col_data){
    if(col_data.size() != col_descs_.size()) throw std::invalid_argument("Invalid insert data");
    void** new_data = new void* [row_col_capacity_];
    int i = 0;
    for (const std::string & str : col_data) {
        if(col_descs_.at(i).second==DataType::kString)  {
            //std::string *st = new std::string(static_cast<std::string>(str));
            //new_data[i] = static_cast<void*>(st);
            new_data[i] = static_cast<void*>(new std::string(static_cast<std::string>(str))); 
            i++; continue;}
        if(col_descs_.at(i).second==DataType::kDouble) {
            new_data[i] = static_cast<void*>(new double(std::stod(static_cast<std::string>(str)))); 
            i++; continue;}
        if(col_descs_.at(i).second==DataType::kInt) {
            new_data[i] = static_cast<void*>(new int(std::stoi(static_cast<std::string>(str)))); 
            i++; continue;}
    }
    rows_[next_unique_id_] = new_data;
    next_unique_id_++;
}

void DbTable::DeleteRowById(unsigned int id){
    if(!rows_.contains(id)) throw std::invalid_argument("Invalid row number");
    for(unsigned long i = 0; i < col_descs_.size(); i++){
        if(col_descs_.at(i).second==DataType::kString) delete static_cast<std::string*>(rows_.at(id)[i]);
        if(col_descs_.at(i).second==DataType::kInt) delete static_cast<int*>(rows_.at(id)[i]);
        if(col_descs_.at(i).second==DataType::kDouble) delete static_cast<double*>(rows_.at(id)[i]);
    } delete[] rows_.at(id);
    rows_.erase(id);
}

DbTable::~DbTable(){
    for (unsigned int row_id = 0; row_id < next_unique_id_; row_id++) {
        if(rows_.contains(row_id)) {
            for(unsigned long i = 0; i < col_descs_.size(); i++){
                if(col_descs_.at(i).second==DataType::kString) delete static_cast<std::string*>(rows_.at(row_id)[i]);
                else if(col_descs_.at(i).second==DataType::kInt) delete static_cast<int*>(rows_.at(row_id)[i]);
                else if(col_descs_.at(i).second==DataType::kDouble) delete static_cast<double*>(rows_.at(row_id)[i]);
            } delete[] rows_.at(row_id);
            rows_.erase(row_id);
        }
    }
    rows_.clear();
    row_col_capacity_ = 2;
    next_unique_id_ = 0;
    col_descs_.clear();
}

std::ostream& operator<<(std::ostream& os, const DbTable& table){
    for(unsigned long i = 0; i < table.col_descs_.size(); i++){
        if(table.col_descs_.at(i).second == DataType::kDouble)  os << table.col_descs_.at(i).first << "(double)";
        if(table.col_descs_.at(i).second == DataType::kInt)     os << table.col_descs_.at(i).first << "(int)";
        if(table.col_descs_.at(i).second == DataType::kString)  os << table.col_descs_.at(i).first << "(std::string)";
        if(i != table.col_descs_.size() - 1) os << ", ";
    } os << std::endl;
    int siz = 0;
    for(unsigned long i = 0; i < table.next_unique_id_; i++){
        if(table.rows_.contains(i)){
            siz++;
            for(unsigned long j = 0; j < table.col_descs_.size(); j++){
                if(table.col_descs_.at(j).second == DataType::kDouble)  {double& d = *(static_cast<double*>(table.rows_.at(i)[j])); os << d;}
                if(table.col_descs_.at(j).second == DataType::kInt)     {int& s = *(static_cast<int*>(table.rows_.at(i)[j])); os << s;}
                if(table.col_descs_.at(j).second == DataType::kString)  {std::string& str = *(static_cast<std::string*>(table.rows_.at(i)[j])); os << str;}
                if(j != table.col_descs_.size() - 1) os << ", ";
            } 
            if(siz != (int)(table.rows_.size() - 1))   os << std::endl;
        }
    }
    return os;
}