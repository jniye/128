#include "db.hpp"

void Database::CreateTable(const std::string& table_name){
    tables_.insert(std::pair<std::string, DbTable*>(table_name, new DbTable())); 
}

void Database::DropTable(const std::string& table_name){
    if(!tables_.contains(table_name)) throw std::invalid_argument("Invalid table name");
    tables_[table_name]->DeleteTable();
    delete tables_.at(table_name);
    tables_.erase(table_name);
}

DbTable& Database::GetTable(const std::string& table_name){
    return *tables_[table_name];
}

Database::Database(const Database& rhs){
    for(auto const& [key, val] : rhs.tables_){
        tables_.insert(std::pair<std::string, DbTable*>(key, new DbTable(*val)));
    }
}

Database& Database::operator=(const Database& rhs){
    if(this == &rhs) return *this;
    for(auto const& [key, val]: tables_){
    val->DeleteTable();
    delete tables_.at(key);
    tables_.erase(key);
    }
    tables_.clear();

    if(rhs.tables_.empty()) return *this;

    for(auto const& [key, val] : rhs.tables_){
        tables_.insert(std::pair<std::string, DbTable*>(key, new DbTable(*val)));
    }
    return *this;

}

Database::~Database(){
    for(auto const& [key, val]: tables_){
    val->DeleteTable();
    delete tables_.at(key);
    tables_.erase(key);
    }
    tables_.clear();
}

std::ostream& operator<<(std::ostream& os, const Database& db){
    for(const std::pair<const std::string, DbTable *>& it: db.tables_){
        os << it.second << std::endl;
    }
    return os;
}