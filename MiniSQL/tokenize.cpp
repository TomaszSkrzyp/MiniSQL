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
        if( std::isalpha(static_cast<unsigned char>(c))){
            std::string word;
            //changed to allow for idemtifiers like user1 etc
            while (i < input.size() &&
                (std::isalnum(static_cast<unsigned char>(input[i])) || input[i] == '_')) {
                word += input[i++];
            }
            std::string upWord=toUpper(word);

            static const std::unordered_set<std::string> keywords = {
                "SELECT", "WHERE", "INSERT", "DELETE", "UPDATE", "ALL"
            };
            if (keywords.count(upWord)){
                    tokens.push_back(Token{TokenType::KEYWORD, upWord});

            }else{

                tokens.push_back(Token{TokenType::IDENTIFIER,upWord});
            }
            
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(c))){
            std::string number;
            while(i<input.size() && std::isdigit(input[i])){
                number+=input[i++];
            }
            tokens.push_back(Token{ TokenType::NUMBER, number });
            continue;
        }
        if (c == '"' || c == '\'') {
            char quoteSign=c;
            i++;
            std::string value;
            while(i<input.size() && input[i]!=quoteSign){
                value+=input[i++];
            }
            if (i < input.size()) i++;
            tokens.push_back(Token{TokenType::STRING, value});
            continue;

        }
        if (c=='='){
            tokens.push_back(Token{TokenType::OPERATOR, "="});
            i++;
            continue;
        }
        i++;
    }
    tokens.push_back({TokenType::END, ""});
    return tokens;

}