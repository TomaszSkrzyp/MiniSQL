#include "storage.h"
#include <fstream>
#include <cstdint>

constexpr size_t RECORD_SIZE =
    sizeof(int) + sizeof(((Record*)0)->name) + sizeof(uint8_t);

void writeRecord(std::ostream& os, const Record& r) {
    os.write(reinterpret_cast<const char*>(&r.id), sizeof(r.id));
    os.write(r.name, sizeof(r.name));

    uint8_t deleted = r.deleted ? 1 : 0;
    os.write(reinterpret_cast<const char*>(&deleted), sizeof(deleted));
}

bool readRecord(std::istream& is, Record& r) {
    if (!is.read(reinterpret_cast<char*>(&r.id), sizeof(r.id)))
        return false;

    if (!is.read(r.name, sizeof(r.name)))
        return false;

    uint8_t deleted;
    if (!is.read(reinterpret_cast<char*>(&deleted), sizeof(deleted)))
        return false;

    r.deleted = (deleted != 0);
    return true;
}

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
        if (!readRecord(file, temp)) break;

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
        while (true) {
            std::streampos currentPos = file.tellg();
            if (!readRecord(file, temp)) break;

            if (temp.deleted) {
                pos = currentPos;
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
        if (!file) return false;
        pos = file.tellp();
    }

    writeRecord(file, record);

    if (file.good() && !record.deleted) {
        index[record.id] = pos;
        return true;
    }

    return false;
}

std::vector<Record> Storage::getAll() {
    std::vector<Record> records;
    std::ifstream file(filename, std::ios::binary);
    if (!file) return records;

    Record temp;
    while (readRecord(file, temp)) {
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
    if (!file) return false;
    file.seekg(it->second);

    if (!readRecord(file, result)) return false;
    return !result.deleted;
}

bool Storage::removeById(int id) {
    auto it = index.find(id);
    if (it == index.end()) return false;

    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) return false;
    file.seekg(it->second);

    Record temp;
    if (!readRecord(file, temp)) return false;

    temp.deleted = true;

    file.seekp(it->second);
    writeRecord(file, temp);

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
    if (!readRecord(file, temp)) return false;

    if (temp.deleted) return false;

    temp.set_name(newName);

    file.seekp(it->second);
    writeRecord(file, temp);

    return true;
}