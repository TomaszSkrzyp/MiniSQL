#pragma once
#include <string>
#include<vector>
enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    STRING,
    OPERATOR,
    END
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> Tokenize(const std::string& input);