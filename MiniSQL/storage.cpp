#include "storage.h"
#include <fstream>

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

void Storage::insert(const Record& record) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    std::streampos pos = file.tellp();

    file.write(reinterpret_cast<const char*>(&record), sizeof(Record));

    if (!record.deleted) {
        index[record.id] = pos;
    }
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