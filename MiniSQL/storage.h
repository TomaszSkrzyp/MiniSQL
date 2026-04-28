
#pragma once
#include "record.h"
#include <vector>
#include <string>
#include <unordered_map>

class Storage {
public:
    Storage(const std::string& filename);

    bool insert(const Record& record);
    std::vector<Record> getAll();
    bool findById(int id, Record& result);
    bool removeById(int id);
    bool updateById(int id, const std::string& newName);

private:
    std::string filename;
    std::unordered_map<int, std::streampos> index;

    void buildIndex();
};