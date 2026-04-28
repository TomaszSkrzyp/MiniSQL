#pragma once
#include <string>

enum class QueryType {
    INSERT,
    SELECT_ONE,
    SELECT_ALL,
    DELETE,
    UPDATE,
    SELECT_WHERE,
    INVALID
};

struct Query {
    QueryType type;
    int id = 0;
    std::string name;
    std::string whereName; 
};