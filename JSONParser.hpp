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
        void next_token(Token &) noexcept;
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
