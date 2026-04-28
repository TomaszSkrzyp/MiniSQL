#include <iostream>
#include <cstring>
#include "storage.h"

int main() {
    Storage storage("data.bin");

    std::string command;
    std::cout << "Commands: insert | list | select | delete\n";
    std::cout << "> ";
    std::cin >> command;

    if (command == "insert") {
        Record r;

        std::cout << "ID: ";
        std::cin >> r.id;

        std::cout << "Name: ";
        std::cin >> std::ws;

        std::string name;
        std::getline(std::cin, name);

        std::strncpy(r.name, name.c_str(), sizeof(r.name));
        r.name[sizeof(r.name) - 1] = '\0'; // prevent overflow

        storage.insert(r);
        std::cout << "Saved.\n";
    }

    else if (command == "list") {
        auto records = storage.getAll();

        for (const auto& r : records) {
            std::cout << r.id << " | " << r.name << "\n";
        }
    }

    else if (command == "select") {
        int id;
        std::cout << "Enter ID: ";
        std::cin >> id;

        Record result;
        if (storage.findById(id, result)) {
            std::cout << result.id << " | " << result.name << "\n";
        }
        else {
            std::cout << "Record not found.\n";
        }
    }
    else if (command == "delete") {
        int id;
        std::cout << "Enter ID: ";
        std::cin >> id;

        if (storage.removeById(id)) {
            std::cout << "Deleted.\n";
        }
        else {
            std::cout << "Record not found.\n";
        }
    }

    else {
        std::cout << "Unknown command.\n";
    }

    return 0;
}