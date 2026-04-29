#pragma once

#include<string>

struct Record {
    int id;
    char name[64];
    bool deleted;

    void set_name(const std::string& src) {
        size_t len = src.copy(name, sizeof(name) - 1);
        name[len] = '\0';
    }
};