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
        if (pos >= tokens.size()) return nullptr;
        return &tokens[pos];
    };

    auto advance = [&]() -> Token* {
        if (pos >= tokens.size()) return nullptr;
        return &tokens[pos++];
    };

    if (tokens.empty()) return q;

    Token* t = peek();
    if (!t || t->type != TokenType::KEYWORD || t->value != "SELECT") {
        return q;
    }
    advance();

    t = peek();
    if (t && t->type == TokenType::KEYWORD && t->value == "ALL") {
        advance();
        q.type = QueryType::SELECT_ALL;
        return q;
    }

    t = peek();
    if (!t || t->type != TokenType::KEYWORD || t->value != "WHERE") {
        return q;
    }
    advance();

    // field
    t = peek();
    if (!t || t->type != TokenType::IDENTIFIER) return q;

    std::string fieldName = toUpper(advance()->value);

    t = peek();
    if (!t || t->type != TokenType::OPERATOR || t->value != "=") return q;
    advance();

    // value
    t = peek();
    if (!t) return q;

    Token valueToken = *advance();

    if (fieldName == "ID" && valueToken.type == TokenType::NUMBER) {
        q.type = QueryType::SELECT_ONE;
        q.id = std::stoi(valueToken.value);
    }
    else if (fieldName == "NAME" && valueToken.type == TokenType::STRING) {
        q.type = QueryType::SELECT_WHERE;
        q.whereName = valueToken.value;
    }
    else {
        q.type = QueryType::INVALID;
    }

    return q;
}