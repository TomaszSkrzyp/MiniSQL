
#include "parser.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include "utils.h"
#include "tokenize.h"
#include <vector>



Query parse(const std::string& input) {
    std::vector<Token> tokens = Tokenize(input);
    Query q;
    q.type = QueryType::INVALID;
    if (tokens.size() < 1) return q;
    std::string command = tokens[0].value;

    if (command == "SELECT") {
        if (tokens.size() >= 2 && tokens[1].value == "ALL") {
            q.type = QueryType::SELECT_ALL;
            return q;
        }

        else if (tokens.size() >= 2 && tokens[1].value == "WHERE") {
            q.type = QueryType::SELECT_WHERE;

            if (tokens[2].type != TokenType::IDENTIFIER) {
                return q;
            }
            std::string fieldName = tokens[2].value;

            if (tokens[3].type != TokenType::OPERATOR || tokens[3].value != "=") {
                return q;
            }
            if (tokens[4].type == TokenType::NUMBER && fieldName=="ID") {
                q.type = QueryType::SELECT_ONE;
                q.id = std::stoi(tokens[4].value);
            }
            else if (tokens[4].type == TokenType::STRING && fieldName == "NAME") {
                q.type = QueryType::SELECT_WHERE; 
                q.whereName = tokens[4].value;
            }
        }
    }

    return q;
}