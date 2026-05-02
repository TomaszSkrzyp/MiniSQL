#pragma once
#include <string>
#include<vector>
enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    STRING,
    OPERATOR,
    SYMBOL,
    END,
    INVALID,
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> Tokenize(const std::string& input);