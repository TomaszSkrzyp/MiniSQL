#include <vector>
#include <cctype>
#include <iostream>
#include "utils.h"
#include "tokenize.h"
std::vector<Token> Tokenize(const std::string& input) {
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < input.size()) {
        char c = input[i];

        if (std::isspace(c)) {
            i++;
            continue;
        }
        if( std::isalpha(c)){
            std::string word;
            while(i<input.size() && std::isalpha(input[i])){
                word+=input[i++];
            }
            std::string upWord=toUpper(word);
            if (upWord == "SELECT" || upWord == "WHERE" || upWord == "INSERT" ||
                upWord == "DELETE" || upWord == "UPDATE" || upWord == "ALL"){
                    tokens.push_back(Token{TokenType::KEYWORD, upWord});

            }else{

                tokens.push_back(Token{TokenType::IDENTIFIER,upWord});
            }
            
            continue;
        }
        if (std::isdigit(c)){
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