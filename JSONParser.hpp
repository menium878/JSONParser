#include <string>
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
    Token literal_error();
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
inline Token Lexer::next_token()noexcept{
    skip_whitespace();
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

