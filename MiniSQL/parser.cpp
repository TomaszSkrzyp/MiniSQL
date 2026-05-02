#include "parser.h"
#include <algorithm>
#include <cctype>
#include "utils.h"
#include "tokenize.h"
#include <vector>

Query parse(const std::string& input) {
    std::vector<Token> tokens = Tokenize(input);
    Query q;
    q.type = QueryType::INVALID;

    size_t pos = 0;
    auto peek = [&]() -> Token* { 
        if (pos>=tokens.size()){
            return nullptr;
        }
        return &tokens[pos]; 
    };
    auto advance = [&]() -> Token* { 
        if (pos>=tokens.size()){
            return nullptr;
        }
        return &tokens[pos]; 
    };

    if (tokens.size() == 0) return q;
    if (tokens[pos].type != TokenType::KEYWORD) return q;

    std::string command = (*advance()).value;
    if (command != "SELECT") return q;

    if (pos >= tokens.size()) return q;

    if ((*peek()).type == TokenType::KEYWORD && (*peek()).value == "ALL") {
        q.type = QueryType::SELECT_ALL;
        return q;
    }

    if ((*peek()).type == TokenType::KEYWORD && (*peek()).value == "WHERE") {
        advance();

        if (pos >= tokens.size() || (*peek()).type != TokenType::IDENTIFIER) return q;
        std::string fieldName = toUpper((*advance()).value);

        if (pos >= tokens.size() || (*peek()).type != TokenType::OPERATOR || (*peek()).value != "=") return q;
        advance();

        if (pos >= tokens.size()) return q;
        Token valueToken = (*advance());

        if (valueToken.type == TokenType::NUMBER && fieldName == "ID") {
            q.type = QueryType::SELECT_ONE;
            q.id = std::stoi(valueToken.value);
        }
        else if (valueToken.type == TokenType::STRING && fieldName == "NAME") {
            q.type = QueryType::SELECT_WHERE;
            q.whereName = valueToken.value;
        }
        if (pos < tokens.size() - 1) {
			q.type = QueryType::INVALID;
        }
    }

    return q;
}