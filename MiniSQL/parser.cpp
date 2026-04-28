#include "parser.h"
#include <sstream>
#include <algorithm>
#include <cctype>   

// helper function to uppercase a string
std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::toupper(c); });
    return result;
}

Query parse(const std::string& input) {
    std::istringstream iss(input);
    std::string command;

    Query q;
    q.type = QueryType::INVALID;

    iss >> command;
    std::string commandUpper = toUpper(command); // normalize

    if (commandUpper == "INSERT") {
        q.type = QueryType::INSERT;
        iss >> q.id >> q.name;
    }
    else if (commandUpper == "SELECT") {
        std::string arg;
        iss >> arg;
        std::string argUpper = toUpper(arg); // normalize

        if (argUpper == "ALL") {
            q.type = QueryType::SELECT_ALL;
        }
        else {
            q.type = QueryType::SELECT_ONE;
            q.id = std::stoi(arg);
        }
    }
    else if (commandUpper == "DELETE") {
        q.type = QueryType::DELETE;
        iss >> q.id;
    }

    return q;
}