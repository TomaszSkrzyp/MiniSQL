#include <iostream>
#include "storage.h"
#include "parser.h"
#include "utils.h"
#include <cstring>

int main() {
    Storage storage("data.bin");
    std::string input;

    while (true) {
        std::cout << "miniSQL > " << std::flush;
        std::getline(std::cin, input); // read full line
        if (input.empty()) continue;

        Query q = parse(input); // parse entire line

        switch (q.type) {
        case QueryType::INSERT: {
            Record r;
            r.id = q.id;
            r.deleted = false;
            std::strncpy(r.name, q.name.c_str(), sizeof(r.name));
            r.name[sizeof(r.name) - 1] = '\0';
            if (storage.insert(r))
                std::cout << "Inserted.\n";
            else
                std::cout << "Error: Duplicate ID.\n";
            break;
        }
        case QueryType::SELECT_ONE: {
            Record r;
            if (storage.findById(q.id, r))
                std::cout << r.id << " | " << r.name << "\n";
            else
                std::cout << "Record not found.\n";
            break;
        }
        case QueryType::SELECT_ALL: {
            auto records = storage.getAll();
            for (const auto& r : records)
                std::cout << r.id << " | " << r.name << "\n";
            break;
        }
        case QueryType::SELECT_WHERE: {
            auto records = storage.getAll();
            bool found = false;
            for (auto& r : records) {
                std::string recName(r.name);
                if (recName == q.whereName) {
                    std::cout << r.id << " | " << r.name << "\n";
                    found = true;
                }
            }
            if (!found) std::cout << "No records found.\n";
            break;
        }
        case QueryType::DELETE: {
            if (storage.removeById(q.id))
                std::cout << "Deleted.\n";
            else
                std::cout << "Record not found.\n";
            break;
        }
        case QueryType::UPDATE: {
            if (storage.updateById(q.id, q.name))
                std::cout << "Updated.\n";
            else
                std::cout << "Record not found.\n";
            break;
        }
        default:
            std::cout << "Invalid query.\n";
        }
    }
}