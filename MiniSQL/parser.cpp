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
    if (!t || t->type != TokenType::KEYWORD ) {
        return q;
    }
    if ( t->value == "SELECT"){
        advance();

        t = peek();
        if (t->type == TokenType::SYMBOL && t->value == "*"){
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
    }
    else if (t->value == "INSERT") {
        advance();

        t = peek();
        if (t && t->value == "INTO") advance();

        t = peek();
        if (t && t->value == "(") {
            while (peek() && peek()->value != ")") advance();
            advance(); 
        }

        t = peek();
        if (!t || t->value != "VALUES") return q;
        advance();

        t = peek();
        if (!t || t->value != "(") return q;
        advance();

        t = peek();
        if (!t || t->type != TokenType::NUMBER) return q;
        q.id = std::stoi(advance()->value);
        t = peek();
        if (t && t->value == ",") advance();

        t = peek();
        if (!t) return q;
        if (t->type == TokenType::STRING || t->type == TokenType::IDENTIFIER) {
            q.name = advance()->value;
        } else {
            return q;
        }

        t = peek();
        if (t && t->value == ")") advance();

        q.type = QueryType::INSERT;
        return q;
    }
    else if (t->value == "UPDATE") {
        advance();

        if (!peek() || peek()->value != "SET") return q;
        advance();

        if (!peek() || toUpper(peek()->value) != "NAME") return q;
        advance();
        
        if (!peek() || peek()->value != "=") return q;
        advance();
        t = peek();
        if (!t || (t->type != TokenType::STRING && t->type != TokenType::IDENTIFIER)) return q;

        q.name = advance()->value;
        if (!peek() || peek()->value != "WHERE") return q;
        advance();

        if (!peek() || toUpper(peek()->value) != "ID") return q;
        advance();
        if (!peek() 
        || peek()->value != "=") return q;
        advance();

        if (!peek() || peek()->type != TokenType::NUMBER) return q;
        q.id = std::stoi(advance()->value);
        q.type = QueryType::UPDATE;
    }
    else if (t->value == "DELETE") {
        advance();
        //workaround: when table names are added this should parse it too
        if (peek() && peek()->value == "FROM") advance();

        if (!peek() || peek()->value != "WHERE") return q;
        advance();

        if (!peek() || toUpper(peek()->value) != "ID") return q;
        advance();

        if (!peek() || peek()->value != "=") return q;
        advance();

        if (!peek() || peek()->type != TokenType::NUMBER) return q;

        q.id = std::stoi(advance()->value);
        q.type = QueryType::DELETE;
    }
    return q;
}