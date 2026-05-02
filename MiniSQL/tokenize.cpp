#include <vector>
#include <cctype>
#include <iostream>
#include "utils.h"
#include "tokenize.h"
#include <unordered_set>

std::vector<Token> Tokenize(const std::string& input) {
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < input.size()) {
        char c = input[i];

        if (std::isspace(static_cast<unsigned char>(c))) {
            i++;
            continue;
        }

        //identifier/keyword
        if (std::isalpha(static_cast<unsigned char>(c))) {
            std::string word;

            while (i < input.size() &&
                   (std::isalnum(static_cast<unsigned char>(input[i])) || input[i] == '_')) {
                word += input[i++];
            }

            std::string upWord = toUpper(word);

            static const std::unordered_set<std::string> keywords = {
                "SELECT", "WHERE", "INSERT", "DELETE", "UPDATE", "ALL"
            };

            if (keywords.count(upWord)) {
                tokens.push_back(Token{TokenType::KEYWORD, upWord});
            } else {
                tokens.push_back(Token{TokenType::IDENTIFIER, word});
            }

            continue;
        }

        //number
        if (std::isdigit(static_cast<unsigned char>(c))) {
            std::string number;

            while (i < input.size() && std::isdigit(static_cast<unsigned char>(input[i]))) {
                number += input[i++];
            }

            tokens.push_back(Token{TokenType::NUMBER, number});
            continue;
        }

        //string
        if (c == '"' || c == '\'') {
            char quoteSign = c;
            i++;

            std::string value;
            bool closed = false;

            while (i < input.size()) {
                if (input[i] == quoteSign) {
                    closed = true;
                    i++;
                    break;
                }
                value += input[i++];
            }

            if (!closed) {
                tokens.push_back(Token{TokenType::INVALID, value});
            } else {
                tokens.push_back(Token{TokenType::STRING, value});
            }

            continue;
        }

        //symbol
        if (c == '*' || c == ';' || c == ',' || c == '(' || c == ')') {
            tokens.push_back(Token{TokenType::SYMBOL, std::string(1, c)});
            i++;
            continue;
        }

        //operator
        if (c == '=' || c == '!' || c == '>' || c == '<') {
            auto next = (i + 1 < input.size()) ? input[i + 1] : '\0';
            std::string op;

            switch (c) {
                case '=':
                    if (next == '=') {
                        op = "==";
                        i += 2;
                    } else {
                        op = "=";
                        i += 1;
                    }
                    break;

                case '!':
                    if (next == '=') {
                        op = "!=";
                        i += 2;
                    } else {
                        op = "!";
                        i += 1;
                    }
                    break;

                case '>':
                    if (next == '=') {
                        op = ">=";
                        i += 2;
                    } else {
                        op = ">";
                        i += 1;
                    }
                    break;

                case '<':
                    if (next == '=') {
                        op = "<=";
                        i += 2;
                    } else {
                        op = "<";
                        i += 1;
                    }
                    break;

                default:
                    tokens.push_back(Token{TokenType::INVALID, std::string(1, c)});
                    i++;
                    continue;
            }

            tokens.push_back(Token{TokenType::OPERATOR, op});
            continue;
        }

        tokens.push_back(Token{TokenType::INVALID, std::string(1, c)});
        i++;
    }

    tokens.push_back(Token{TokenType::END, ""});
    return tokens;
}