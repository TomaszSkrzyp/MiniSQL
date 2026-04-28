#include "storage.h"
#include <fstream>
#include <cstring>

Storage::Storage(const std::string& filename) : filename(filename) {
    buildIndex();
}

void Storage::buildIndex() {
    std::ifstream file(filename, std::ios::binary);

    if (!file) return;

    Record temp;
    std::streampos pos;

    while (true) {
        pos = file.tellg();
        if (!file.read(reinterpret_cast<char*>(&temp), sizeof(Record)))
            break;

        if (!temp.deleted) {
            index[temp.id] = pos;
        }
    }
}

bool Storage::insert(const Record& record) {
    if (index.find(record.id) != index.end()) {
        return false;
    }

    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    std::streampos pos;
    bool foundSlot = false;

    if (file) {
        Record temp;
        while (file.read(reinterpret_cast<char*>(&temp), sizeof(Record))) {
            if (temp.deleted) {
                pos = file.tellg();
                pos -= static_cast<std::streamoff>(sizeof(Record));
                foundSlot = true;
                break;
            }
        }
    }

    if (foundSlot) {
        file.clear();
        file.seekp(pos);
    } else {
        file.close();
        file.open(filename, std::ios::binary | std::ios::out | std::ios::app);
        file.seekp(0, std::ios::end);
        pos = file.tellp();
    }

    if (!file) return false;

    file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
    
    if (file.good() && !record.deleted) {
        index[record.id] = pos;
        return true;
    }
    return false;
}

std::vector<Record> Storage::getAll() {
    std::vector<Record> records;
    std::ifstream file(filename, std::ios::binary);

    Record temp;
    while (file.read(reinterpret_cast<char*>(&temp), sizeof(Record))) {
        if (!temp.deleted) {
            records.push_back(temp);
        }
    }

    return records;
}

bool Storage::findById(int id, Record& result) {
    auto it = index.find(id);
    if (it == index.end()) return false;

    std::ifstream file(filename, std::ios::binary);
    file.seekg(it->second);

    file.read(reinterpret_cast<char*>(&result), sizeof(Record));
    return !result.deleted;
}

bool Storage::removeById(int id) {
    auto it = index.find(id);
    if (it == index.end()) return false;

    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    file.seekp(it->second);

    Record temp;
    file.read(reinterpret_cast<char*>(&temp), sizeof(Record));

    temp.deleted = true;

    file.seekp(it->second);
    file.write(reinterpret_cast<const char*>(&temp), sizeof(Record));

    index.erase(id);
    return true;
}

bool Storage::updateById(int id, const std::string& newName) {
    auto it = index.find(id);
    if (it == index.end()) return false;

    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) return false;

    file.seekg(it->second);

    Record temp;
    file.read(reinterpret_cast<char*>(&temp), sizeof(Record));

    if (temp.deleted) return false;

    std::strncpy(temp.name, newName.c_str(), sizeof(temp.name));
    temp.name[sizeof(temp.name) - 1] = '\0';

    file.seekp(it->second);
    file.write(reinterpret_cast<const char*>(&temp), sizeof(Record));

    return true;
}