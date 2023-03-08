#include "db.hpp"

void Database::CreateTable(const std::string& table_name){
    tables_.insert({table_name, new DbTable()});
}

void Database::DropTable(const std::string& table_name){
    if(!tables_.contains(table_name)) throw std::invalid_argument("Invalid table name");
    //tables_[table_name]->DeleteTable();
    delete tables_.at(table_name);
    tables_.erase(table_name);
}

DbTable& Database::GetTable(const std::string& table_name){
    return *tables_[table_name];
}

Database::Database(const Database& rhs){
    for (const auto& i : rhs.tables_)
        tables_.insert({i.first, new DbTable(*i.second)});
}

Database& Database::operator=(const Database& rhs){
    if(this == &rhs) return *this;

    for(auto const& i: tables_){
    delete i.second;
    tables_.erase(i.first);
    }
    tables_.clear();

    for (const auto& i : rhs.tables_)
        tables_.insert({i.first, new DbTable(*i.second)});

    return *this;
}

Database::~Database(){
    for(auto const& i: tables_){
    delete i.second;
    tables_.erase(i.first);
    }
    tables_.clear();
}

std::ostream& operator<<(std::ostream& os, const Database& db){
    for(const std::pair<const std::string, DbTable *>& it: db.tables_){
        os << it.second << std::endl;
    }
    return os;
}