#include "parser.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include "utils.h"


Query parse(const std::string& input) {
    std::istringstream iss(input);
    std::string command;
    Query q;
    q.type = QueryType::INVALID;

    iss >> command;
    std::string commandUpper = toUpper(command);

    if (commandUpper == "INSERT") {
        q.type = QueryType::INSERT;
        iss >> q.id >> q.name;
    }
    else if (commandUpper == "SELECT") {
        std::string arg;
        iss >> arg;
        std::string argUpper = toUpper(arg);

        if (argUpper == "ALL") {
            q.type = QueryType::SELECT_ALL;
        }
        else if (argUpper == "WHERE") {
            q.type = QueryType::SELECT_WHERE;

            std::string rest;
            std::getline(iss, rest);

            // remove spaces
            rest.erase(remove_if(rest.begin(), rest.end(), ::isspace), rest.end());

            auto pos = rest.find('=');
            if (pos == std::string::npos) {
                q.type = QueryType::INVALID;
                return q;
            }

            std::string field = rest.substr(0, pos);
            std::string value = rest.substr(pos + 1);

            if (toUpper(field) == "NAME") {
                q.whereName = value;
            }
            else {
                q.type = QueryType::INVALID;
            }
        }
    }
    else if (commandUpper == "DELETE") {
        q.type = QueryType::DELETE;
        iss >> q.id;
    }
    else if (commandUpper == "UPDATE") {
        q.type = QueryType::UPDATE;
        iss >> q.id >> q.name;
    }

    return q;
}