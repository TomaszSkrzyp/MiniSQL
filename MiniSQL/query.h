#pragma once
#include <string>

enum class QueryType {
    INSERT,
    SELECT_ONE,
    SELECT_ALL,
    DELETE,
    INVALID
};

struct Query {
    QueryType type;
    int id;
    std::string name;
};