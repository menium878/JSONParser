#include <string>
#include <iostream>
#include <string_view>
#include <variant>
enum class TokenType {
    LEFT_BRACE,    // {
    RIGHT_BRACE,   // }
    LEFT_BRACKET,  // [
    RIGHT_BRACKET, // ]
    COLON,         // :
    COMMA,         // ,
    STRING,
    NUMBER,
    TRUE, FALSE, NULL_VALUE,
    END_OF_INPUT,
    ERROR
};
using TokenValue = std::variant<std::monostate, double, std::string, bool, std::nullptr_t>;
struct Token {
    TokenType type;
    TokenValue text;
};

class Lexer{
public:
    Lexer(std::string_view input) : input_(input), pos_(0) {}
    Token next_token() noexcept;
private:
    std::string_view input_;
    size_t pos_;

    char peek() const;
    char advance();
    void skip_whitespace();
    Token string();
    Token number();
    Token literal_or_error();
};

inline void Lexer::skip_whitespace(){
    while (isspace(peek())) advance();
}
inline char Lexer::peek() const{
    return pos_ < input_.size() ? input_[pos_]: '\0';
}
inline char Lexer::advance(){
    return pos_ < input_.size() ? input_[pos_++]: '\0';
}
inline Token Lexer::literal_or_error() {
    if (input_.substr(pos_, 4) == "true") {
        pos_ += 4;
        return {TokenType::TRUE, "true"};
    }
    if (input_.substr(pos_, 5) == "false") {
        pos_ += 5;
        return {TokenType::FALSE, "false"};
    }
    if (input_.substr(pos_, 4) == "null") {
        pos_ += 4;
        return {TokenType::NULL_VALUE, "null"};
    }
    return {TokenType::ERROR, ""};
}
inline Token Lexer::string() {
    std::string result;
    while (true) {
        char c = advance();
        if (c == '"') break;
        if (c == '\\') {
            char esc = advance();
            switch (esc) {
                case '"': result += '"'; break;
                case 'n': result += '\n'; break;
                default: result += esc;
            }
        } else {
            result += c;
        }
    }
    return {TokenType::STRING, result};
}
inline Token Lexer::number() {
    size_t start = pos_;
    while (isdigit(peek()) || peek() == '.' || peek() == '-' || peek() == '+') advance();
    return {TokenType::NUMBER, std::string(input_.substr(start - 1, pos_ - start + 1))};
}
inline Token Lexer::next_token()noexcept{
    skip_whitespace();
    if (pos_ >= input_.size()) {
    return {TokenType::END_OF_INPUT, {}};
}

    char c = advance();

    switch (c) {
        case '{': return {TokenType::LEFT_BRACE, "{"};
        case '}': return {TokenType::RIGHT_BRACE, "}"};
        case '[': return {TokenType::LEFT_BRACKET, "["};
        case ']': return {TokenType::RIGHT_BRACKET, "]"};
        case ':': return {TokenType::COLON, ":"};
        case ',': return {TokenType::COMMA, ","};
        case '"': return string();
        case 't': case 'f': case 'n':
            --pos_; // Rewind so we can read full word
            return literal_or_error();
        default:
            if (isdigit(c) || c == '-') {
                --pos_;
                return number();
            }
            return {TokenType::ERROR, std::string(1, c)};
    }
}

inline std::string token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::COLON: return "COLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::STRING: return "STRING";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::TRUE: return "TRUE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::NULL_VALUE: return "NULL";
        case TokenType::END_OF_INPUT: return "END_OF_INPUT";
        case TokenType::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

inline void print_token(const Token& token) {
    std::cout << token_type_to_string(token.type) << ": ";

    std::visit([](auto&& val) {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, std::string>)
            std::cout << val;
        else if constexpr (std::is_same_v<T, double>)
            std::cout << val;
        else if constexpr (std::is_same_v<T, bool>)
            std::cout << (val ? "true" : "false");
        else if constexpr (std::is_same_v<T, std::nullptr_t>)
            std::cout << "null";
        else
            std::cout << "none";
    }, token.text);

    std::cout << "\n";
}

