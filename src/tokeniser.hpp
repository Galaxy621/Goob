#ifndef TOKENISER_HPP
#define TOKENISER_HPP

#include <string>
#include <sstream>
#include <vector>
#include <cstdint>

namespace Goob {
    extern std::vector<char> Operators;
    extern std::vector<char> Arithmetic;
    extern std::vector<std::string> Keywords;
    
    bool is_operator(char value);
    bool is_arithmetic(char value);

    class Input {
        public:
        Input(std::string input, bool destroy_newlines);
        ~Input() {};

        char get_char(int32_t optional_index, bool add_to_cache);
        char peek_char();
        char review_char();

        bool has_next_char() const;

        void increment();
        void decrement();
        void clear_cache();

        std::string pull_cache(bool clear_cache);
        std::string get_input() const;

        private:
        std::string m_Cache;
        std::string m_Input;
        uint32_t m_Pos;
    };

    class Token {
        public:
        Token(std::string type, std::string value);
        ~Token() {};

        std::string get_type() const;
        std::string get_value() const;

        operator std::string() const {
            std::stringstream sstm;
            sstm << "Goob::Token(type=\"" << this -> type << "\", value=\"" << this -> value << "\")";
            return sstm.str();
        }

        private:
        std::string type;
        std::string value;
    
    };

    class Tokeniser {
        public:
        Tokeniser(std::string program);
        ~Tokeniser() {};
        std::vector<Token> get_tokens();

        private:
        std::vector<Token> tokens;
        void pull_from_cache(Input*);
    };
}

std::ostream& operator<<(std::ostream& stream, const Goob::Token& token);

                    // case '!':
                    //     if (cInput.peek_char() == '[')
                    //         isComment = true;
                    //     break;

                    // case '[':
                    //     if (cInput.review_char() == '!')
                    //         break;

                    // case ']':
                    //     if (isComment)
                    //         break;

#endif