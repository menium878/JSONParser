#include "JSONParser.hpp"

int main() {
    std::string input = R"({"key": "value", "num": 42, "bool": true, "nullval": null})";
    Lexer lexer(input);

    Token token;
    while ((token = lexer.next_token()).type != TokenType::ERROR && token.type != TokenType::END_OF_INPUT) {
        print_token(token);
    }

    if (token.type == TokenType::ERROR) {
        std::cout << "Lexing error encountered.\n";
    }

    return 0;
}
